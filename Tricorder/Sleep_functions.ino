void prepSleep()
{
  DDRD &= B00000011;       // set Arduino pins 2 to 7 as inputs, leaves 0 & 1 (RX & TX) as is
  DDRB = B00000000;        // set pins 8 to 13 as inputs
  PORTD |= B11111100;      // enable pullups on pins 2 to 7, leave pins 0 and 1 alone
  PORTB |= B11111111;      // enable pullups on pins 8 to 13
}

void pinInterrupt(void)
{
  /* This will bring us back from sleep. */

  /* We detach the interrupt to stop it from
   * continuously firing while the interrupt pin
   * is low.
   */
}

void sleepNow()
{
  prepSleep();
  // Choose preferred sleep mode:
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);

  // Set sleep enable (SE) bit:
  sleep_enable();
  stopLCD();

  // Set pin 2 as interrupt and attach handler:
  attachInterrupt(0, pinInterrupt, LOW);
  delay(150);

  // Put the device to sleep:

  sleep_mode();

  // Upon waking up, sketch continues from this point.
  sleep_disable();

  detachInterrupt(0);
  startLCD();
}
