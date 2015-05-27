void timerIsr() {
  encoder->service();
}
void beginEncoders()
{
  encoder = new ClickEncoder(4, 3, 2);         //start encoder
  Timer1.initialize(1000);
  Timer1.attachInterrupt(timerIsr);
  last = -1;
}

uint16_t readEncoders(uint8_t type)
{
  //Read encoder for sample type
  value += encoder->getValue();

  if (value != last) {
    last = value;
    Serial.print("Encoder Value: ");
    Serial.println(value);
    if (type == 1) {
      value = constrain(value, 0, 16);
      val = value;
      Serial.println(val);
      if (val <= 0)
        val = 0;
      if (val >= 1 && val <= 4)
        val = 1;
      if (value >= 5 && val <= 8)
        val = 2;
      if (value >= 9 && val <= 12)
        val = 3;
      if (val >= 13 && val <= 16)
        val = 4;
      switch (val) {
        case 0:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Sample Type: ");
          lcd.print(" ");
          break;
        case 1:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Sample Type: ");
          lcd.setCursor(0, 1);
          lcd.print("Spider w.Litter");
          break;
        case 2:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Sample Type: ");
          lcd.setCursor(0, 1);
          lcd.print("Spider w/oLitter");
          break;
        case 3:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Sample Type: ");
          lcd.setCursor(0, 1);
          lcd.print("Artif. w.Litter");
          break;
        case 4:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Sample Type: ");
          lcd.setCursor(0, 1);
          lcd.print("Artif. w/oLitter");
          break;
      }
    }
    else if (type == 2) {
      val = constrain(value, 0, 1028);
      val = map(val, 0, 1028, 0, 256);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Sample number: ");
      lcd.setCursor(0, 1);
      lcd.print(val);
    }

  }

  ClickEncoder::Button b = encoder->getButton();
  if (b != ClickEncoder::Open) {
    Serial.print("Button: ");
#define VERBOSECASE(label) case label: Serial.println(#label); break;
    switch (b) {
        VERBOSECASE(ClickEncoder::Pressed);
        VERBOSECASE(ClickEncoder::Held)
        VERBOSECASE(ClickEncoder::Released)
        VERBOSECASE(ClickEncoder::DoubleClicked)
      case ClickEncoder::Clicked:
        Serial.println(F("ClickEncoder::Clicked"));
        if (type == 1)
          _sType = true;
        else if (type == 2)
          _sNo = true;
        return val;
        break;
    }
  }
}
