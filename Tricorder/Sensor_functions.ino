/*Sensor functions*/
void startSensors()
{
#if GPSattached
  if (!startGPS())                                          //If GPS doesn't report ready, wait till it is ready.
  {
    debug.println("GPS failed, or not present");           //Don't do anything more
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.println(F("GPS.01x01"));                           //Error code on LCD
    return;
  }
#else if RTCattached
  if (! rtc.isrunning())                                   //If RTC is not running
  {
    debug.println("RTC is NOT running!");                  //Report RTC failure
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.println(F("RTC.01x01"));                           //Error code on LCD
  }
#endif
#if SDattached
  if (!sd.begin(chipSelect, SPI_FULL_SPEED))              //Use SPI_HALF_SPEED if on a breadboard to prevent bus errors
  {
    sd.initErrorHalt();
    debug.println("Card failed, or not present");         //Don't do anything more
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.println(F("SD.01X01"));                           //Error code on LCD
  }
  makeFile();
#endif
#if TSL2591attached
  if (!tsl.begin()) 
  {
    debug.println("No sensor found ... check your wiring?");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.println(F("TSL2591.01x01"));                     //Error code on LCD
    return;
  }
  configureTSL2591();
#endif
#if TSL2561attached
  if(!tsl.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    debug.print("Ooops, no TSL2561 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  configureTSL2561();
#endif
#if HTU21DFattached
  if (!htu.begin())
  {
    debug.println("Couldn't find sensor!");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.println(F("HTU21DF.01x01"));                     //Error code on LCD
    while (1);
  }
#endif
#if AM2315attached
if (! am2315.begin())
{
    debug.println("Sensor not found, check wiring & pullups!");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.println(F("AM2315.01x01"));                     //Error code on LCD
     while (1);
  }
  #endif
  debug.println("Sensors started!");
}

void readSensors()
{
  readSoil();
  blinkBusy(2);
  delay(1000);
  readAir();
  blinkBusy(2);
  delay(1000);
  readLight();
  blinkBusy(2);
  delay(1000);
}

void takeReading()
{
  lcd.clear();
  lcd.setCursor (0,1);
  lcd.print(F("Reading sensors..."));
  readSensors();
  uint16_t readDelay = 6000;                           //Delay between readings is set to readDelay
  debug.println(F("Reading sensors..."));
  uint32_t readBegin = millis();
  readSensors();                                     //Reads all sensors
  shiftVariables();                                  //Shifts variables to old variables
  readSensors();                                     //Reads all sensors
  lcd.clear();
  lcd.setCursor (0,1);
  lcd.print(F("Read complete!"));
  delay(200);
}


void confirmReading()
{
  uint32_t loopBegin = millis();                     //Declare variable to mark beginning of time count
  uint16_t timeout = 180000;

  takeReading();
  while (!compareVariables())                       //If past and present variables are not similar
  {
    takeReading();

    if (!timeElapsed(timeout, loopBegin))           //If 3 minutes haven't elapsed since the loop began
        error();
  }

  logData();                                       //Logs data to SD card
  resetVariables();                                //Shifts variables to old variables
}


