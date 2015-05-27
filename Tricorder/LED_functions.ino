/* LED Functions */

//The flashLED() function takes the LED number (if single LED, value is 1), color code, the number of repeats, time between repeats, as arguments and flashes the LED the required number of times. Refer to LED_colors for the supported colors

void flashLED(uint8_t numLED, uint8_t color, uint8_t repeat, uint16_t time)
{
  byte i = 0;
  while (i<repeat)
  {
    ledIO(numLED, color);
    delay(time);
    ledIO(numLED, 0);
    delay(time);
    i++;
  }
  delay(time*2);
}


void blinkSuccess(uint8_t repeats)
{
  flashLED(0, 2, repeats, 50);
}

void blinkError(uint8_t repeats)
{
  flashLED(0, 1, repeats, 50);
}

void blinkBusy(uint8_t repeats)
{
  flashLED(0, 3, repeats, 50);
}
