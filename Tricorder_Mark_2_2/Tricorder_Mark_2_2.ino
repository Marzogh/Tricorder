//Libraries
#include <TinyGPS++.h>                           //For GPS
#include <SoftwareSerial.h>                      //For GPS
#include <Wire.h>                                //For I2C devices
#include <LCD.h>                                 //For LCD display
#include <LiquidCrystal_I2C.h>                   //For LCD display
#include <SD.h>                                  //For SD card
#include <Adafruit_TSL2591.h>                    //For Light sensor
#include <Adafruit_Sensor.h>                     //For Light sensor
#include <Adafruit_HTU21DF.h>                    //For Air Temperature and Humidity sensor
#include <SHT1x.h>                               //For Soil Temperature and Moisture sensor

//#include <EEPROM.h>                              //For EEPROM access


//Definitions
#define DEBUG true                               //Permit all debugging code to be run


//Variables
bool newRead = false;
float prev_airT, prev_airH, airT, airH;
float prev_soilT, prev_soilH, soilT, soilH;
char filename[13] = {0};
uint16_t ir, full;
uint32_t lum;


//Pins
const uint8_t RLED = 5;                           //Red LED pin
const uint8_t GLED = 6;                           //Blue LED pin
const uint8_t BLED = 7;                           //Green LED pin
const uint8_t chipSelect = 10;                    //SD card chipselect pin
const uint8_t sampleSelect = 4;                   //Sample selection (in this case - spider or artificial hole) - an SPST switch is connected to this pin
const uint8_t clockPin = A2;                      //Clock pin for Soil sensor (SHT1X)
const uint8_t dataPin = A3;                       //Data pin for Soil sensor (SHT1X)


//Constants
static const int RXPin = 7, TXPin = 8;            //TX and RX pins for Software Serial for GPS
static const uint32_t GPSBaud = 9600;             //Set GPS Baud rate


//Initiate Objects
TinyGPSPlus gps;                                  //Initiate TinyGPS object for GPS
LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7);        //Initiate LiquidCrystal object for LCD. (0x27 is the I2C bus address for an unmodified I2C backpack)
File logfile;                                     //Initiate File object for SD card
Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591);    //Initiate TSL2591 object for light sensor
Adafruit_HTU21DF htu = Adafruit_HTU21DF();        //Initiate HTU21DF object for air sensor
SHT1x sht1x(dataPin, clockPin);                   //Initiate SHT1X object for soil sensor


//Declarations
//SoftwareSerial GPS(RXPin, TXPin);                 //Declare Software Serial
HardwareSerial GPS = Serial1;
HardwareSerial debug = Serial;                    //Declare debug from Hardware Serial


void setup()
{
#if DEBUG
  debug.begin(9600);                              //Begin debug Hardware Serial stream
#endif
  GPS.begin(GPSBaud);                             //Begins GPS Software Serial stream
  Wire.begin();                                   //Begin I2C library

  pinMode (10, OUTPUT);                           //Set SD chipselect pin to output
  pinMode (sampleSelect, INPUT);                  //Set the sampleSelect pin to INPUT
  digitalWrite (sampleSelect, HIGH);              //Turn on pull-up resistors for the sampleSelect pin
  pinMode (2, INPUT);                             //Set interrupt pin to INPUT
  digitalWrite (2, HIGH);                         //Turn on pull-up resistors for pin 2
  attachInterrupt(0, readButton, LOW);            //Attach interrupt to pin 2 to detect change of state to LOW

  startLCD();                                     //Starts the LCD
  startSensors();                                 //Starts the sensors
}

void loop()
{
  if (newRead == false)
  {
    debug.println(F("Take a reading..."));
    lcd.home();
    lcd.print(F("Take a reading..."));                //LCD shows message
  }
  else
  {
    uint32_t loopBegin = millis();                 //Declare variable to mark beginning of time count
    if (!timeElapsed(180000, loopBegin))           //If 3 minutes haven't elapsed since the loop began
    {
      while (!compareVariables())                  //If past and present variables are not similar
      {
        uint16_t readDelay = 5000;                 //Delay between readings is set to delay
        readSensors();                             //Reads all sensors
        shiftVariables();                          //Shifts variables to old variables
        uint32_t readBegin = millis();
        while (!timeElapsed(readDelay, readBegin)) //While time elapsed < default delay between readings
        {
          flashLED(3, 1);                           //Flash busy LED once
        }
        readSensors();
      }
      logData();                                    //Logs data to SD card
      shiftVariables();                             //Shifts variables to old variables
    }
    else
    {
      error();
    }
  }
}


