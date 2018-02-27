#include <DS3231.h>
#include <Adafruit_BMP280.h>
#include <dht11.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
const int chipSelect = 10;

byte deg[8] = {
  B11000,
  B11000,
  B00111,
  B01000,
  B01000,
  B01000,
  B00111,
  B00000
};
byte three[8]={
  B11100,
  B00010,
  B01100,
  B00010,
  B11100,
  B00000,
  B00000,
  B00000
};
byte two[8] = {
  B01100,
  B10010,
  B00100,
  B01000,
  B11110,
  B00000,
  B00000,
  B00000
};
byte b4[8] = {
  B01110,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B00000
};
byte b3[8] = {
  B01110,
  B11111,
  B10001,
  B11111,
  B11111,
  B11111,
  B11111,
  B00000
};
byte b2[8] = {
  B01110,
  B11111,
  B10001,
  B10001,
  B11111,
  B11111,
  B11111,
  B00000
};
byte b1[8] = {
  B01110,
  B11111,
  B10001,
  B10001,
  B10001,
  B11111,
  B11111,
  B00000
};
byte b0[8] = {
  B01110,
  B11111,
  B10001,
  B10001,
  B10001,
  B10001,
  B11111,
  B00000
};

dht11 DHT11;
const int DHT_PIN = 2;
DS3231 Clock;
Adafruit_BMP280 bmp;
LiquidCrystal_I2C lcd(0x3F,16,2);
const int LCD_SWITCH_PIN=3;
const int UV_OUT = A0;
const int UV_3V3 = A1;
const int BAT_PIN=A7;

const int DUST_SAMPLING_TIME = 280; 
const int DUST_DELTA_TIME = 40; 
const int DUST_SLEEP_TIME = 9680; 
const int DUST_LED_PIN=4;
const int DUST_OUT_PIN=A2;

const int INTERVAL=60000;
unsigned long ms=0;
unsigned long now=0;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  //LCD Initialize
  lcd.init();
  lcd.noDisplay();
  lcd.createChar(0, deg); 
  lcd.createChar(1, two); 
  lcd.createChar(5, three);
  lcd.createChar(2, b4); 
  lcd.createChar(3, b2); 
  lcd.createChar(4, b0); 
  //LCD Switch Initialize
  pinMode(LCD_SWITCH_PIN, INPUT_PULLUP);
  //BMP280 Initialize
  bmp.begin();
  //UV Initialize
  pinMode(UV_OUT, INPUT);
  pinMode(UV_3V3, INPUT);
  //
  pinMode(DUST_LED_PIN,OUTPUT);
  //SD Initialize
  if (SD.begin(chipSelect))
  {
    Serial.println("SD initialize done.");
    if (!SD.exists("datalog.txt")) {
      File dataFile = SD.open("datalog.txt", FILE_WRITE);
      dataFile.println("Time,Temperature(℃),Humidity(%),Pressure(Pa),UV(mW/cm^2),Dust(mg/m3)");
      dataFile.close();
    }
    WriteSD();
  }
}

void loop() {
 /* Serial.println(ReadTime());
  Serial.println(ReadDHT11());
  Serial.println(ReadBMP());
  Serial.println(ReadUV());
  Serial.println();
  delay(1000);*/

  //Detect LCD switch press
  if(digitalRead(LCD_SWITCH_PIN)==LOW){
    lcd.backlight();
    lcd.display();
    Display();
    Display();
    lcd.noBacklight();
    lcd.noDisplay();
  }

  //Write SD
  now=millis();
  if(abs(now-ms)>INTERVAL){
    ms=now;
    WriteSD();
  }
}

int ReadBattery(){
  int val=analogRead(BAT_PIN);
  float volts=(val/1023.0)*5;
  int per=(volts-3.2)*100;
  int res=4;
  if(per>=60){
    res=2;
  }
  else if(per>=20&&per<60){
    res=3;
  }
  else{
    res=4;
  }
  return res;
}

void Display(){
  bool h12,PM,Century;
  int second=0,minute=0,hour=0,date=0,month=0,year=0; 
  String secondDis="",minuteDis="",hourDis="",dateDis="",monthDis="";
  int dustLevel=0;
  float temp=0,press=0,hum=0,dust=0,dustRaw=0;
  String tempDis="",pressDis="",humDis="",uvDis="",dustLevelDis="",timeDis="",resDis="";

  //read time
  second=Clock.getSecond();
  minute=Clock.getMinute();
  hour=Clock.getHour(h12, PM);
  date=Clock.getDate();
  month=Clock.getMonth(Century);
  year=Clock.getYear();

  if(second<10)
    secondDis=secondDis+"0"+second;
  else
    secondDis+=second;
  if(minute<10)
    minuteDis=minuteDis+"0"+minute;
  else
    minuteDis+=minute;
  if(hour<10)
    hourDis=hourDis+"0"+hour;
  else
    hourDis+=hour;
  if(date<10)
    dateDis=dateDis+"0"+date;
  else
    dateDis+=date;
  if(month<10)
    monthDis=monthDis+"0"+month;
  else
    monthDis+=month;
  timeDis=timeDis+year+"/"+monthDis+"/"+dateDis+" "+hourDis+":"+minuteDis;//+":"+secondDis;

  //read bmp280
  temp=bmp.readTemperature();
  press=bmp.readPressure();

  tempDis=tempDis+"Temp:"+temp;
  pressDis=pressDis+"Pres:"+press+"Pa";
  
  //print in LCD
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(timeDis);
  lcd.setCursor(15,0);
  lcd.write(ReadBattery());
  lcd.setCursor(0,1);
  lcd.print(tempDis);
  lcd.write(0);
  delay(5000);

  //read dht11
  int chk = DHT11.read(DHT_PIN);
  switch (chk)
  {
    case DHTLIB_OK: 
      hum=DHT11.humidity;
      break;
    default: 
      hum=0; 
      break;
  }
  humDis=humDis+"Humi:"+hum+"%";
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(humDis);
  lcd.setCursor(0,1);
  lcd.print(pressDis);
  delay(5000);

  //read uv
  int uvLevel = averageAnalogRead(UV_OUT);
  int refLevel = averageAnalogRead(UV_3V3);
  float outputVoltage = 3.3 / refLevel * uvLevel;
  float uvIntensity = mapfloat(outputVoltage, 0.99, 2.9, 0.0, 15.0);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("UV(mW/cm");
  lcd.write(1);
  lcd.print("):");
  lcd.print(uvIntensity);
  lcd.setCursor(0,1);
  lcd.print("Level:");
  lcd.print((int)(uvIntensity*10*0.04));
  delay(5000);

  //read sharp
  dustRaw=ReadDust();
  dustLevel=((dustRaw/1024.0)-0.0356)*120000*0.035;
  if(dustLevel>=3000){
    dustLevelDis="Very Bad";
  }
  else if(dustLevel>=1050&&dustLevel<3000){
    dustLevelDis="Bad";
  }
  else if(dustLevel>=300&&dustLevel<1050){
    dustLevelDis="Medium";
  }
  else if(dustLevel>=150&&dustLevel<300){
    dustLevelDis="Good";
  }
  else if(dustLevel>=75&&dustLevel<150){
    dustLevelDis="Very Good";
  }
  else if(dustLevel>=0&&dustLevel<75){
    dustLevelDis="Perfect";
  }
  else{
    dustLevelDis="Error";
  }
  float calcVoltage = dustRaw * (5.0 / 1024.0);
  dust = 0.17 * calcVoltage - 0.1;
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Dust(mg/m");
  lcd.write(5);
  lcd.print("):");
  lcd.print(dust);
  lcd.setCursor(0,1);
  lcd.print("Level:");
  lcd.print(dustLevelDis);
  delay(5000);
}

void WriteSD(){
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  dataFile.print(ReadTime());
  dataFile.print(",");
  dataFile.print(bmp.readTemperature());
  dataFile.print(",");
  DHT11.read(DHT_PIN);
  dataFile.print(DHT11.humidity);
  dataFile.print(",");
  dataFile.print(bmp.readPressure());
  dataFile.print(",");
  int uvLevel = averageAnalogRead(UV_OUT);
  int refLevel = averageAnalogRead(UV_3V3);
  float outputVoltage = 3.3 / refLevel * uvLevel;
  float uvIntensity = mapfloat(outputVoltage, 0.99, 2.9, 0.0, 15.0);
  dataFile.print(uvIntensity);
  dataFile.print(",");
  dataFile.println(ReadDustForSD());
  dataFile.close();
}

/***************************************
 * Method for Serial.print()
****************************************/
float ReadDust(){
  float voMeasured = 0; //Raw Signal Value (0-1023)

  digitalWrite(DUST_LED_PIN,LOW); // power on the LED
  delayMicroseconds(DUST_SAMPLING_TIME);

  voMeasured = analogRead(DUST_OUT_PIN); // read the dust value

  delayMicroseconds(DUST_DELTA_TIME);
  digitalWrite(DUST_LED_PIN,HIGH); // turn the LED off
  delayMicroseconds(DUST_SLEEP_TIME);

  return voMeasured;
} 

float ReadDustForSD(){
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
  String time="";
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
    
  time=time+"20"+year+"/"+monthStr+"/"+dateStr+" "+hourStr+":"+minuteStr+":"+secondStr;

  return time;
}

String ReadBMP(){
  String res="";
  float temp=0,press=0,alt=0;

  temp=bmp.readTemperature();
  press=bmp.readPressure();
  
  res=res+"Temperature:"+temp+"℃"+"  Pressure:"+press+"Pa";

  return res;
}

String ReadDHT11(){
  String res="";
  int chk = DHT11.read(DHT_PIN);
  switch (chk)
  {
    case DHTLIB_OK: 
      break;
    case DHTLIB_ERROR_CHECKSUM: 
      return "Checksum error"; 
    case DHTLIB_ERROR_TIMEOUT: 
      return "Time out error"; 
    default: 
      return "Unknown error"; 
  }

  res=res+"Humidity:"+DHT11.humidity+"%  Temperature:"+DHT11.temperature+"℃";

  return res;
}

String ReadUV(){
  String res="";
  int uvLevel = averageAnalogRead(UV_OUT);
  int refLevel = averageAnalogRead(UV_3V3);
  float outputVoltage = 3.3 / refLevel * uvLevel;
  float uvIntensity = mapfloat(outputVoltage, 0.99, 2.9, 0.0, 15.0);

  res=res+"UV Intensity (mW/cm^2):"+uvIntensity;

  return res;
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


