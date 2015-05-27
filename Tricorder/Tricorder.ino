/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Definitions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#define SERIAL_ECHO

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Libraries ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2591.h>
#include <SHT1x.h>
#include "HTU21D.h"
#include <RTClib.h>
#include <SdFat.h>
#include <LCD.h>                                 //For LCD display
#include <LiquidCrystal_I2C.h>                   //For LCD display
#include <Adafruit_NeoPixel.h>
#include <ClickEncoder.h>
#include <TimerOne.h>
#include <avr/sleep.h>
#include <avr/power.h>                           // Power management

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~ Pin definitions ~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#define dataPin A3
#define clockPin A2
#define chipSelect 10
#define LEDPIN     9
#define NUMPIXELS  1
#define startupDelay 1000

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Objects ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591);
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7); //Initiate LiquidCrystal object for LCD. (0x27 is the I2C bus address for an unmodified I2C backpack)
SHT1x sht1x(dataPin, clockPin);
HTU21D air;
RTC_DS1307 rtc;
SdFat sd;
SdFile logfile;
Adafruit_NeoPixel LED = Adafruit_NeoPixel(NUMPIXELS, LEDPIN, NEO_GRB + NEO_KHZ800);
ClickEncoder *encoder;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
float soilH, soilT, airH, airT;
uint32_t lux;
char filename[13] = {0};
uint8_t sType, sNo = 0;
boolean _sType, _sNo = false;
byte val;
int16_t last, value;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Setup ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void setup()
{
#ifdef SERIAL_ECHO
  Serial.begin(38400);                            //Begin Serial
#endif
  startLCD();                                     //Starts the LCD
  LED.begin();
  splashScreen();                                 //Shows splash screen
  blinkBusy(1);                                    //Blinks busy LED
  pinMode (chipSelect, OUTPUT);                   //Set SD chipselect pin to output
  //pinMode (sampleSelect, INPUT_PULLUP);           //Set the sampleSelect pin to INPUT and turn on pull-up resistors for the sampleSelect pin
  pinMode (2, INPUT_PULLUP);                      //Set  pin 2 (interrupt pin 0) to INPUT and turn on pull-up resistors
  pinMode (3, INPUT_PULLUP);                      //Set  pin 3 (interrupt pin 1) to INPUT and turn on pull-up resistors
  tsl.begin();                                    //TSL2591 begin
  configureSensor();
  Serial.println(F("TSL2591 initialised"));
  air.begin();                                    //HTU21DF begin
  Serial.println(F("HTU21D initialised"));
  rtc.begin();                                    //RTC begin
  Serial.println(F("RTC initialised"));
  if (!rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    blinkError(3);
  }
  if (!sd.begin(chipSelect, SPI_FULL_SPEED)) {    //SD begin
    sd.initErrorHalt();
  }
  delay(startupDelay);
  beginEncoders();
}

void loop()
{
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Encoder code ~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sample Type: ");
  do {
    sType = readEncoders(1);
  } while (!_sType);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sample number: ");
  do {
    sNo = readEncoders(2);
    //Serial.print(F("Sample Number = "));
    //Serial.println(sNo);
  } while (!_sNo);
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  initialLCDState();
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~ TSL2561 code ~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  readLight();
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ SHT1x code ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  readSoil();
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~ HTU21DF code ~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  readAir();
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ SD code ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  makeFile();
  logData();
  displaySensors(3);
  sleepNow();
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
}
