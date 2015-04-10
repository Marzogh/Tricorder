/* LED Functions */

//The flashLED() function takes the color code and the number of repeats as arguments and flashes the LED the required number of times. Make sure the color is input in lower case.
//refer to LED_colors for the supported colors

void flashLED(uint8_t color, uint8_t repeat, uint16_t time)
{
  byte i = 0;
  while (i<repeat)
  {
    ledIO(color);
    pixels.show();
    delay(time);
    ledIO(0);
    delay(time);
    i++;
  }
}


void blinkSuccess(uint8_t repeats)
{
  flashLED(2, repeats, 50);
}

void blinkError(uint8_t repeats)
{
  flashLED(1, repeats, 50);
}

void blinkBusy(uint8_t repeats)
{
  flashLED(3, repeats, 50);
}
