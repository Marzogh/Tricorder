// The ledIO() function turns on or off an LED (in a particular color - if RGB) based on the mode passed to it. The mode can be the color of LED - in lower case - if RGB
//or simply on or off if a single color LED
void ledIO(uint8_t mode)
{
  switch (mode) {
    case 1:                                            //RED
      pixels.setPixelColor(0, pixels.Color(255, 0, 0));
      break;

    case 2:                                            //GREEN
      pixels.setPixelColor(0, pixels.Color(0, 255, 0));
      break;

    case 3:                                            //BLUE
      pixels.setPixelColor(0, pixels.Color(0, 0, 255));
      break;

    case 4:                                            //PURPLE
      pixels.setPixelColor(0, pixels.Color(127, 0, 127));
      break;

    case 5:                                            //TEAL
      pixels.setPixelColor(0, pixels.Color(0, 127, 127));
      break;

    case 6:                                            //ORANGE
      pixels.setPixelColor(0, pixels.Color(255, 0, 50));
      break;

    case 7:                                            //WHITE
      pixels.setPixelColor(0, pixels.Color(255, 127, 127));
      break;

    case 0:                                            //OFF
      pixels.setPixelColor(0, pixels.Color(0, 0, 0));
  }
}

