/***************************
SD Card
  MOSI - pin 11
  MISO - pin 12
  CLK - pin 13
  CS - pin 10

UV Sensor
  VIN - 3.3V
  OUT - A0
  GND - GND
  EN - 3.3V
  3.3V - A1

Dust Sensor
  Sharp pin 1 (V-LED) - 5V (串联150Ω电阻，pin 1和电阻之间连220μF电容接地)
  Sharp pin 2 (LED-GND) - GND
  Sharp pin 3 (LED) - pin 4
  Sharp pin 4 (S-GND) - GND
  Sharp pin 5 (Vo) - A2
  Sharp pin 6 (Vcc) - 5V

ESP8266
  TX - pin 3
  RX - pin 2
  CH_PD - pin 5
  VCC - 3.3V
  GND - GND

I2C Sensors
  VCC - 5V
  GND - GND
  SDA - A4
  SCL - A5
**************************/

#include <avr/pgmspace.h>
#include <SoftwareSerial.h>
#include <DS3231.h>
#include <Adafruit_BMP280.h>
#include <AM2320.h>
#include <SD.h>

const int SD_CS = 10;

DS3231 Clock;
AM2320 th;
Adafruit_BMP280 bmp;

const int UV_OUT = A0;
const int UV_3V3 = A1;

const int DUST_LED_PIN = 4;
const int DUST_OUT_PIN = A2;
const int DUST_SAMPLING_TIME = 280;
const int DUST_DELTA_TIME = 40;
const int DUST_SLEEP_TIME = 9680;

SoftwareSerial esp(3, 2);
const int CH_PD_PIN = 5;

bool Century=false;

void setup() {
  //BMP280 Initialize
  bmp.begin();

  //UV Initialize
  pinMode(UV_OUT, INPUT);
  pinMode(UV_3V3, INPUT);

  //Dust Initialize
  pinMode(DUST_LED_PIN, OUTPUT);

  //ESP8266 Initialize
  EspInit();

  //SD Initialize
  if (SD.begin(SD_CS))
  {
    /*if (!SD.exists("datalog.txt"))
    {
      File dataFile = SD.open("datalog.txt", FILE_WRITE);
      dataFile.println("Time,Temperature(℃),Humidity(%),Pressure(Pa),UV(mW/cm2),Dust(mg/m3)");
      dataFile.close();
    }*/
    
    WriteSdAndPost();
  }
}

void loop() {
  // 10 分钟传送一次数据
  // The interval of data transmission is 10 minutes
  int minute=Clock.getMinute();
  if(minute % 10 == 0)
    WriteSdAndPost();
  delay(60000);
}

void WriteSdAndPost() {
  String fileName=(String)"20"+Clock.getYear()+"_"+Clock.getMonth(Century)+"_"+Clock.getDate();

  File dataFile = SD.open(fileName, FILE_WRITE);
  String datetime = ReadTime();
  dataFile.print(datetime);
  dataFile.print(",");

  th.Read();
  dataFile.print(th.t);
  dataFile.print(",");
  dataFile.print(th.h);
  dataFile.print(",");

  float p = bmp.readPressure();
  dataFile.print(p);
  dataFile.print(",");

  float uv = ReadUV();
  dataFile.print(uv);
  dataFile.print(",");

  float dust = ReadDust();
  dataFile.println(dust);
  dataFile.close();

  // 单个字符串太长，拆分成3个
  // The string is too long, so I split it into 3.
  String data1 = (String)"DateTime=" + datetime;
  String data2 = (String)"&Temperature=" + th.t + "&Humidity=" + th.h;
  String data3 = (String)"&Pressure=" + p + "&Dust=" + dust + "&UV=" + uv;

  Post(data1, data2, data3);
}

void Post(String data1, String data2, String data3) {
  // 连接至 WIFI
  // Connect to WIFI
  esp.println(F("AT+CWJAP=\"YOUR WIFI NAME\",\"YOUR WIFI PASSWORD\""));        // 替换你的 WIFI 名称和密码 - Replace your WIFI name and password
  delay(5000);

  // 连接至服务器
  // Connect to servers
  esp.println(F("AT+CIPSTART=\"TCP\",\"YOUR SERVER ADDRESS\",PORT"));        // 替换你的服务器地址和端口 - Replace your server address and port
  delay(2000);
  
  esp.println(F("AT+CIPMODE=1"));
  delay(200);
  
  esp.println(F("AT+CIPSEND"));       // 开始传输 - begin
  delay(200);

  // POST 请求报文
  // POST request message
  esp.print(F("POST /api/Weather?Token=YOUR TOKEN&"));       // 替换你的 Token - Replace your Token
  esp.print(data1);
  esp.print(data2);
  esp.print(data3);
  esp.print(F(" HTTP/1.1\r\n"));
  esp.print(F("Host: YOUR HOST\r\n"));       // 替换你的 Host - Replace your Host
  esp.print(F("Content-Length: 0\r\n"));
  esp.println();
  delay(500);

  esp.print(F("+++"));        // 结束传输 - exit
  delay(1000);

  esp.println(F("AT+CIPMODE=0"));
  delay(200);
  
  esp.println(F("AT+CIPCLOSE"));
  delay(200);
}

void EspInit() {
  // 设置 CH_PD_PIN 为高电平
  // Set CH_PD_PIN to high level
  pinMode(CH_PD_PIN, OUTPUT);
  digitalWrite(CH_PD_PIN, HIGH);
  delay(1000);

  // 初始化软串口
  // SoftwareSerial init
  esp.begin(9600);
  delay(200);

  // 重启 ESP8266
  // Reboot ESP8266
  esp.println(F("AT+RST"));
  delay(5000);

  // STA 模式
  esp.println(F("AT+CWMODE=1"));
  delay(2000);

  esp.println(F("AT+CIPMUX=0"));
  delay(200);
}

float ReadUV() {
  int uvLevel = averageAnalogRead(UV_OUT);
  int refLevel = averageAnalogRead(UV_3V3);
  float outputVoltage = 3.3 / refLevel * uvLevel;
  float uvIntensity = mapfloat(outputVoltage, 0.99, 2.9, 0.0, 15.0);

  return uvIntensity;
}

float ReadDust() {
  float voMeasured = 0;
  float calcVoltage = 0;
  float dustDensity = 0;

  digitalWrite(DUST_LED_PIN, LOW);
  delayMicroseconds(DUST_SAMPLING_TIME);

  voMeasured = analogRead(DUST_OUT_PIN);

  delayMicroseconds(DUST_DELTA_TIME);
  digitalWrite(DUST_LED_PIN, HIGH);
  delayMicroseconds(DUST_SLEEP_TIME);

  calcVoltage = (float)voMeasured * (5.0 / 1024.0);

  if(calcVoltage >= 0.6)
    dustDensity = 0.17 * calcVoltage - 0.1;
  else
    dustDensity = 0;

  return dustDensity;
}

String ReadTime() {
  bool h12, PM;
  int second = 0, minute = 0, hour = 0, date = 0, month = 0, year = 0;
  //String secondStr="",minuteStr="",hourStr="",dateStr="",monthStr="";

  second = Clock.getSecond();
  minute = Clock.getMinute();
  hour = Clock.getHour(h12, PM);
  date = Clock.getDate();
  month = Clock.getMonth(Century);
  year = Clock.getYear();

  /*if(second<10)
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
    monthStr+=month;*/

  // %20 表示“空格”，用在 url 中
  // %20 denotes "space" and is used in url
  String time = (String)"20" + year + "/" + month + "/" + date + "%20" + hour + ":" + minute + ":" + second;

  return time;
}

int averageAnalogRead(int pinToRead){
  byte numberOfReadings = 8;
  unsigned int runningValue = 0;

  for (int x = 0 ; x < numberOfReadings ; x++)
    runningValue += analogRead(pinToRead);
  runningValue /= numberOfReadings;

  return (runningValue);
}

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max){
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
