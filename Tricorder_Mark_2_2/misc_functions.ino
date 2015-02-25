
/* Functions are in the following order
 --> LED Functions
 --> Sensor functions
 --> Time Functions
 --> Miscellaneous Functions
 */


/* LED Functions */

//The flashLED() function takes the color code and the number of repeats as arguments and flashes the LED the required number of times. Make sure the color is input in lower case.
//refer to LED_colors for the supported colors
void flashLED(uint8_t color, byte repeat)
{
  byte i = 0;
  while (i < repeat)
  {
    ledIO(color);
    delay(100);
    ledIO(0);
    i++;
  }
}


/*Sensor functions*/
void startSensors()
{
  if (!startGPS())                                //If GPS doesn't report ready, wait till it is ready.
  {
    debug.println("GPS failed, or not present");   //Don't do anything more:
    return;
  }
  
  if (!SD.begin(chipSelect)) 
  {
    debug.println("Card failed, or not present");   //Don't do anything more:
    //return;
  }
  makeFile();
  
  if (!tsl.begin()) 
  {
    debug.println("No sensor found ... check your wiring?");
    return;
  }
  
  if (!htu.begin())
  {
    Serial.println("Couldn't find sensor!");
    while (1);
  }
}

void readSensors()
{
  readSoil();
  delay(250);
  readLight();
  delay(250);
  readAir();
  delay(250);
}

/*Time functions*/

//The timeElapsed() function takes the wait time required (in milliseconds) & the millis() read when the timecount began as arguments and indicates when the wait time has elapsed.
bool timeElapsed(uint16_t waitTime, uint32_t prevTime)
{
  if (millis() > (prevTime+waitTime))
    return true;
  else
    return false;
}


/* Miscellaneous functions */

void error()
{
  flashLED(1, 3);
  delay(100);
  ledIO(1);
  lcd.clear();
  lcd.home();
  lcd.print(F("Device error!"));
  lcd.setCursor (0,1);
  lcd.print(F("Please powercycle..."));
  while (1);
}

void powerOff()
{
  resetVariables();
  lcd.print(F("Shutting down..."));
  flashLED(6, 20);
  lcd.noDisplay();
}

void readButton()
{
  delay(200);
  if (digitalRead(2) == HIGH)
    newRead = true;
  else
  {
    flashLED(4, 28);
    if (digitalRead(2) == LOW)
      powerOff();
  }
}

boolean sType()
{
  if (digitalRead(sampleSelect) == LOW)
    return true;
  else
    return false;
}

long readVcc() 
{
  // Read 1.1V reference against AVcc
  // set the reference to Vcc and the measurement to the internal 1.1V reference
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
  ADMUX = _BV(MUX5) | _BV(MUX0);
#elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
  ADMUX = _BV(MUX3) | _BV(MUX2);
#else
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#endif  

  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Start conversion
  while (bit_is_set(ADCSRA,ADSC)); // measuring

  uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH  
  uint8_t high = ADCH; // unlocks both

  long result = (high<<8) | low;

  result = 1125300L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
  return result; // Vcc in millivolts
}





