/***************************
 * SD Card
 * MOSI - pin 11
 * MISO - pin 12
 * CLK - pin 13
 * CS - pin 10
 *
 * UV Sensor
 * VIN - 3.3V
 * OUT - A0
 * GND - GND
 * EN - 3.3V
 * 3.3V - A1
 * 
 * Dust Sensor
 * Sharp pin 1 (V-LED) - 5V (串联150Ω电阻，pin 1和电阻之间连220μF电容接地)
 * Sharp pin 2 (LED-GND) - GND
 * Sharp pin 3 (LED) - pin 2 
 * Sharp pin 4 (S-GND) - GND
 * Sharp pin 5 (Vo) - A0
 * Sharp pin 6 (Vcc) - 5V
 * 
 * I2C Sensors
 * VCC - 5V
 * GND - GND
 * SDA - A4
 * SCL - A5
 **************************/

#include <SoftwareSerial.h>
#include <DS3231.h>
#include <Adafruit_BMP280.h>
#include <AM2320.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
const int chipSelect = 10;

DS3231 Clock;
AM2320 th;
Adafruit_BMP280 bmp;

const int UV_OUT = A0;
const int UV_3V3 = A1;

const int DUST_SAMPLING_TIME = 280; 
const int DUST_DELTA_TIME = 40; 
const int DUST_SLEEP_TIME = 9680; 
const int DUST_LED_PIN = 4;
const int DUST_OUT_PIN = A2;

SoftwareSerial esp(9, 8); // RX, TX
const int CH_PD_PIN = 7;
const String ssid ="MERCURY_ZHANG";
const String pwd ="85859095";
const String host="weatherstationapi.zhangyue.xin";
const String path="/api/Weather?";
const String token="xxxxx";

/*const unsigned long INTERVAL=600000;//120000;
unsigned long last=0;
unsigned long now=0;
unsigned long minus=0;*/

void setup() {
  Serial.begin(9600);
  Wire.begin();

 //ESP8266 Initialize
 // EspInit();
  
  //BMP280 Initialize
  bmp.begin();
  
  //UV Initialize
  pinMode(UV_OUT, INPUT);
  pinMode(UV_3V3, INPUT);
  
  //Dust Initialize
  pinMode(DUST_LED_PIN,OUTPUT);
  
  //SD Initialize
  if (SD.begin(chipSelect))
  {
    Serial.println("SD initialize done.");
    if (!SD.exists("datalog.txt"))
    {
      File dataFile = SD.open("datalog.txt", FILE_WRITE);
      dataFile.println("Time,Temperature(℃),Humidity(%),Pressure(Pa),UV(mW/cm2),Dust(mg/m3)");
      dataFile.close();
    }
    WriteSdAndPost();
  }
}

void loop() {
  /*now=millis();
  minus=abs(now-last);
  if(minus>INTERVAL)
  {
    last=now;
    WriteSD();
  }*/

  /*int minute=Clock.getMinute();
  if(minute % 10 == 0)
    WriteSdAndPost();
  delay(60000);*/

  WriteSdAndPost();
  delay(5000);
}

void Post(String data){
  esp.println("AT+CIPSTART=\"TCP\",\"weatherstationapi.zhangyue.xin\",80");
  delay(1000);
  esp.println("AT+CIPSEND");
  delay(10);

  esp.print("POST "+path+data+" HTTP/1.1\r\n");
  esp.print("Host: "+host+"\r\n");
  esp.print("Content-Length: 0\r\n");
  esp.print("\r\n");
  delay(200);

  esp.print("+++");  
  esp.println("AT+CIPCLOSE");
  delay(10);
}

void WriteSdAndPost(){
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  String dateStr=ReadTime();
  Serial.println(dateStr);
  dataFile.print(dateStr);
  dataFile.print(",");

  th.Read();
  dataFile.print(th.t);
  dataFile.print(",");
  dataFile.print(th.h);
  dataFile.print(",");

  float p=bmp.readPressure();
  dataFile.print(p);
  dataFile.print(",");

  float uv=ReadUV();
  dataFile.print(uv);
  dataFile.print(",");

  float dust=ReadDust();
  dataFile.println(dust);
  dataFile.close();

  /*String data="Token="+token+"&Temperature="+th.t+"&Humidity="+th.h+"&Pressure="+p+"&UV="+uv+"&Dust"+dust;
  Post(data);
  Serial.println(data);*/
}

void EspInit(){
  pinMode(CH_PD_PIN,OUTPUT);
  digitalWrite(CH_PD_PIN,HIGH);
  delay(100);
  esp.begin(9600);
  esp.println("AT+RST");
  delay(1000);
  String connect=(String)"AT+CWJAP=\""+ssid+"\",\""+pwd+"\"";
  esp.println(connect);
  delay(1000);
  esp.write("AT+CIPMUX=0");
  delay(10);
  esp.println("AT+CIPMODE=1");
  delay(10);
}

float ReadUV(){
  int uvLevel = averageAnalogRead(UV_OUT);
  int refLevel = averageAnalogRead(UV_3V3);
  float outputVoltage = 3.3 / refLevel * uvLevel;
  float uvIntensity = mapfloat(outputVoltage, 0.99, 2.9, 0.0, 15.0);

  return uvIntensity;
}

float ReadDust(){
  float voMeasured = 0; //Raw Signal Value (0-1023)
  float calcVoltage = 0; //Voltage
  float dustDensity = 0;
  
  digitalWrite(DUST_LED_PIN,LOW); // power on the LED
  delayMicroseconds(DUST_SAMPLING_TIME);

  voMeasured = analogRead(DUST_OUT_PIN); // read the dust value

  delayMicroseconds(DUST_DELTA_TIME);
  digitalWrite(DUST_LED_PIN,HIGH); // turn the LED off
  delayMicroseconds(DUST_SLEEP_TIME);

  // 0 - 5V mapped to 0 - 1023 integer values
  // recover voltage
  calcVoltage = (float)voMeasured * (5.0 / 1024.0);

  // linear eqaution taken from http://www.howmuchsnow.com/arduino/airquality/
  // Chris Nafis (c) 2012
  //level=((voMeasured/1024)-0.0356)*120000*0.035
  //dustDensity = 0.17 * calcVoltage - 0.1;
  if ( calcVoltage >= 0.6 )
  {
      dustDensity = 0.17 * calcVoltage - 0.1;
  }
  else
  {
      dustDensity = 0;
  }

  return dustDensity;
} 

String ReadTime(){
  String dateTimeStr="";
  bool h12,PM,Century;
  int second=0,minute=0,hour=0,date=0,month=0,year=0; 
  String secondStr="",minuteStr="",hourStr="",dateStr="",monthStr="";
  
  second=Clock.getSecond();
  minute=Clock.getMinute();
  hour=Clock.getHour(h12, PM);
  date=Clock.getDate();
  month=Clock.getMonth(Century);
  year=Clock.getYear();

  if(second<10)
    secondStr=secondStr+"0"+second;
  else
    secondStr+=second;
  if(minute<10)
    minuteStr=minuteStr+"0"+minute;
  else
    minuteStr+=minute;
  if(hour<10)
    hourStr=hourStr+"0"+hour;
  else
    hourStr+=hour;
  if(date<10)
    dateStr=dateStr+"0"+date;
  else
    dateStr+=date;
  if(month<10)
    monthStr=monthStr+"0"+month;
  else
    monthStr+=month;

  dateTimeStr=dateTimeStr+"20"+year+"/"+monthStr+"/"+dateStr+" "+hourStr+":"+minuteStr+":"+secondStr;

  return dateTimeStr;
}

int averageAnalogRead(int pinToRead)
{
  byte numberOfReadings = 8;
  unsigned int runningValue = 0; 

  for(int x = 0 ; x < numberOfReadings ; x++)
    runningValue += analogRead(pinToRead);
  runningValue /= numberOfReadings;

  return(runningValue);  
}

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
