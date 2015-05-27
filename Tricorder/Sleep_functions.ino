/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Sleep Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void pinInterrupt(void)
{

}

void sleepNow()
{
  stopLCD();
  /*DDRD &= B00000011;       // set Arduino pins 2 to 7 as inputs, leaves 0 & 1 (RX & TX) as is
  DDRB = B00000000;        // set pins 8 to 13 as inputs
  PORTD |= B00000100;      // enable pullups on pin 2, leave pins the rest alone
  PORTB |= B00000000;      // disable pullups on pins 8 to 13*/
  pinMode(2, INPUT_PULLUP);
  ADCSRA = 0;              // disable ADC
  power_all_disable();

  noInterrupts();          //A timed sequence follows and cannot be interrupted
  // Set pin 2 as interrupt and attach handler:
  attachInterrupt(0, pinInterrupt, LOW);
  delay(150);

  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // Choose preferred sleep mode:
  sleep_enable();          // Set sleep enable (SE) bit:

  // turn off brown-out enable in software
  MCUCR = bit (BODS)
          | bit (BODSE);   // turn on brown-out enable select
  MCUCR = bit (BODS);      // this must be done within 4 clock cycles of above

  // We are guaranteed that the sleep_cpu call will be done as the processor executes the next instruction after interrupts are turned on.
  interrupts();

  // Put the device to sleep:
  sleep_cpu();

  // Upon waking up, sketch continues from this point.
  sleep_disable();       // This will bring us back from sleep.
  detachInterrupt(0);    //We detach the interrupt to stop it from continuously firing while the interrupt pin is low.
  power_all_enable();
  startLCD();
  resetVariables();

}
void resetVariables()
{
  soilH, soilT, airH, airT = 0;
  lux = 0;
  sType, sNo = 0;
  _sType, _sNo = false;
}

