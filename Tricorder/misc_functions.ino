
/* Functions are in the following order
 --> Time Functions
 --> Miscellaneous Functions
 */

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
  flashLED(1, 3, 50);
  delay(100);
  ledIO(1);
  lcd.clear();
  lcd.home();
  lcd.print(F("Device error!"));
  lcd.setCursor (0,1);
  lcd.print(F("Please powercycle..."));
  while (1);
}

/*void readButton()
{
  newRead = !newRead;
}

void sampleType()
{
  if (digitalRead(3) == LOW)
  stype = 0;
  else
  stype = 1;
}*/

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








