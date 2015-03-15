  // The ledIO() function turns on or off an LED (in a particular color - if RGB) based on the mode passed to it. The mode can be the color of LED - in lower case - if RGB
  //or simply on or off if a single color LED
  void ledIO(uint8_t mode)
  {
    switch (mode) {
    case 1:                                            //RED
      digitalWrite(RLED, HIGH);
      digitalWrite(GLED, LOW);
      digitalWrite(BLED, LOW);
      break;
  
    case 2:                                            //GREEN
      digitalWrite(RLED, LOW);
      digitalWrite(GLED, HIGH);
      digitalWrite(BLED, LOW);
      break;
  
    case 3:                                            //BLUE
      digitalWrite(RLED, LOW);
      digitalWrite(GLED, LOW);
      digitalWrite(BLED, HIGH);
      break;
  
    case 4:                                            //PURPLE
      analogWrite(RLED, 127);
      analogWrite(GLED, 0);
      analogWrite(BLED, 127);
      break;
  
    case 5:                                            //TEAL
      analogWrite(RLED, 0);
      analogWrite(BLED, 127);
      analogWrite(GLED, 127);
      break;
  
    case 6:                                            //ORANGE
      analogWrite(RLED, 255);
      analogWrite(BLED, 0);
      analogWrite(GLED, 50);
      break;
  
    case 7:                                            //WHITE
      digitalWrite(RLED, HIGH);
      analogWrite(BLED, 127);
      analogWrite(GLED, 127);
      break;
  
    case 0:                                            //OFF
      digitalWrite(RLED, LOW);
      digitalWrite(BLED, LOW);
      digitalWrite(GLED, LOW);
    }
  }

