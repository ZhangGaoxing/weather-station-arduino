# Weather Station
## Introduction
This project includes an Arduino-based weather station, an ASP.NET Core MVC data presentation page, and an ASP.NET Core WebAPI. The collected data is sent using ESP8266 and written to the SQL Server database via WebAPI. The weather station consists of the following parts:

* Arduino Nano
* Nano Shield
* AM2320 - Digital Temperature and Humidity Sensor
* BMP280 - Barometric Pressure and Altitude Sensor
* ML8511 - UV sensor
* GP2Y1010F45 - Optical Dust Sensor
* DS3231 - RTC
* ESP8266 - WIFI
* SD card module
* 18650 Li-ion and Charger (Optional)

![](https://raw.githubusercontent.com/ZhangGaoxing/weather-station-arduino/master/Img/flowchart.png)

Most of the referenced libraries come from Adafruit and Aduino Playground, and some I don't know who written it. AM2320 comes from [thakshak](https://github.com/thakshak/AM2320).

## Components
||Name|Count|
|---|:---|---:|
|<img src="https://raw.githubusercontent.com/ZhangGaoxing/weather-station-arduino/master/Img/ArduinoNano.jpg" width="50" />|Arduino Nano|x1|
|<img src="https://raw.githubusercontent.com/ZhangGaoxing/weather-station-arduino/master/Img/NanoShield.jpg" width="50" />|Nano Shield|x1|
|<img src="https://raw.githubusercontent.com/ZhangGaoxing/weather-station-arduino/master/Img/AM2320.jpg" width="50" />|AM2320|x1|
|<img src="https://raw.githubusercontent.com/ZhangGaoxing/weather-station-arduino/master/Img/BMP280.jpg" width="50" />|BMP280|x1|
|<img src="https://raw.githubusercontent.com/ZhangGaoxing/weather-station-arduino/master/Img/ML8511.jpg" width="50" />|ML8511|x1|
|<img src="https://raw.githubusercontent.com/ZhangGaoxing/weather-station-arduino/master/Img/GP2Y1010F45.jpg" width="50" />|GP2Y1010F45|x1|
|<img src="https://raw.githubusercontent.com/ZhangGaoxing/weather-station-arduino/master/Img/DS3231.jpg" width="50" />|DS3231|x1|
|<img src="https://raw.githubusercontent.com/ZhangGaoxing/weather-station-arduino/master/Img/ESP8266.jpg" width="50" />|ESP8266|x1|
|<img src="https://raw.githubusercontent.com/ZhangGaoxing/weather-station-arduino/master/Img/SD.jpg" width="50" />|SD card module|x1|
|<img src="https://raw.githubusercontent.com/ZhangGaoxing/weather-station-arduino/master/Img/18650.jpg" width="50" />|18650 Li-ion and Charger (Optional)|x1|
||Male/Female Jumper Wires|some|

## Apps
||Name|
|---|---|
|<img src="https://raw.githubusercontent.com/ZhangGaoxing/weather-station-arduino/master/Img/ArduinoIDE.jpg" />|Arduino IDE|
|<img src="https://raw.githubusercontent.com/ZhangGaoxing/weather-station-arduino/master/Img/VS2017.jpg" />|Microsoft Visual Studio 2017|
||Microsoft SQL Server|

## Arduino Connection
* I2C Sensors
  * VCC - 5V
  * GND - GND
  * SDA - A4
  * SCL - A5
  
* ML8511
  * VIN - 3.3V
  * OUT - A0
  * GND - GND
  * EN - 3.3V
  * 3.3V - A1

* GP2Y1010F45
  * Sharp pin 1 (V-LED) - 5V (Series 150Ω resistor, pin 1 and resistance connected to 220 μF capacitor and grounded)
  * Sharp pin 2 (LED-GND) - GND
  * Sharp pin 3 (LED) - pin 4
  * Sharp pin 4 (S-GND) - GND
  * Sharp pin 5 (Vo) - A2
  * Sharp pin 6 (Vcc) - 5V

* ESP8266
  * TX - pin 3
  * RX - pin 2
  * CH_PD - pin 5
  * VCC - 3.3V
  * GND - GND

* SD Card
  * MOSI - pin 11
  * MISO - pin 12
  * CLK - pin 13
  * CS - pin 10

## How to Use
Connect sensors to Arduino.

![](https://raw.githubusercontent.com/ZhangGaoxing/weather-station-arduino/master/Img/S1.jpg)

### In ASP.NET Core
1. Open "**WeatherStation.sln**" and rebuild solution
   
    ![](https://raw.githubusercontent.com/ZhangGaoxing/weather-station-arduino/master/Img/ASP1.jpg)


2. In project **API**, **WebPage**, open **Startup.cs**, change connection string

    ![](https://raw.githubusercontent.com/ZhangGaoxing/weather-station-arduino/master/Img/ASP2.jpg)

3. In project **API**, open Controllers - **WeatherController.cs**, change token

    ![](https://raw.githubusercontent.com/ZhangGaoxing/weather-station-arduino/master/Img/ASP3.jpg)

4. Open Tools - NuGet Package Manager - **Package Manager Console**, change **Default project** to **Utility**
   
    ![](https://raw.githubusercontent.com/ZhangGaoxing/weather-station-arduino/master/Img/ASP41.jpg)
    ![](https://raw.githubusercontent.com/ZhangGaoxing/weather-station-arduino/master/Img/ASP42.jpg)

    in PMC run this
    ```
    Add-Migration Initial
    Update-Database
    ```
    or use SSMS run this SQL
    ```SQL
    CREATE DATABASE WeatherStation

    CREATE TABLE [dbo].[Weathers] (
        [ID]          INT           IDENTITY (1, 1) NOT NULL,
        [DateTime]    DATETIME2 (7) NOT NULL,
        [Temperature] FLOAT (53)    NOT NULL,
        [Humidity]    FLOAT (53)    NOT NULL,
        [Pressure]    FLOAT (53)    NOT NULL,
        [Dust]        FLOAT (53)    NOT NULL,
        [UV]          FLOAT (53)    NOT NULL,
        CONSTRAINT [PK_Weathers] PRIMARY KEY CLUSTERED ([ID] ASC)
    );
    ```

5. Publish **API**, **WebPage** to your server

### In Arduino
1. Copy libraries to **x:\Program Files (x86)\Arduino\libraries**
2. Open "**WeatherStation.ino**"
3. In **Line 134**, replace your WIFI name and password
4. In **Line 139**, replace your server address and port
5. In **Line 150**, replace your Token
6. In **Line 155**, replace your Host

    ![](https://raw.githubusercontent.com/ZhangGaoxing/weather-station-arduino/master/Img/Arduino1.jpg)

7. Upload program to your Arduino

## Demo Page
[http://weatherstation.zhangyue.xin/](http://weatherstation.zhangyue.xin)

![](https://raw.githubusercontent.com/ZhangGaoxing/weather-station-arduino/master/Img/demopage.jpg)
