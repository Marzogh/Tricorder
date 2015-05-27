// The ledIO() function turns on or off an LED (in a particular color - if RGB) based on the mode passed to it. The mode can be the color of LED - in lower case - if RGB
//or simply on or off if a single color LED
void ledIO(uint8_t number, uint8_t mode)
{
  uint8_t i = number;
  switch (mode) {
    case 1:                                            //RED
      LED.setPixelColor(i, LED.Color(255, 0, 0));
      LED.show();
      break;

    case 2:                                            //GREEN
      LED.setPixelColor(i, LED.Color(0, 255, 0));
      LED.show();
      break;

    case 3:                                            //BLUE
      LED.setPixelColor(i, LED.Color(0, 0, 255));
      LED.show();
      break;

    case 4:                                            //PURPLE
      
      LED.setPixelColor(i, LED.Color(127, 0, 127));
      LED.show();
      break;

    case 5:                                            //TEAL
      LED.setPixelColor(i, LED.Color(0, 127, 127));
      LED.show();
      break;

    case 6:                                            //ORANGE
      LED.setPixelColor(i, LED.Color(255, 50, 0));
      LED.show();
      break;

    case 7:                                            //WHITE
      LED.setPixelColor(i, LED.Color(127, 127, 127));
      LED.show();
      break;

    case 0:                                            //OFF
      LED.setPixelColor(i, LED.Color(0, 0, 0));
      LED.show();
  }
}

