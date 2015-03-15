/*

int analogAverageRead(float pinToRead)
{
  byte numberOfReadings = 8;
  unsigned int runningValue = 0; 

  for(int x = 0 ; x < numberOfReadings ; x++)
    runningValue += analogRead (pinToRead)
  runningValue /= numberOfReadings;

  return(runningValue);  
}

void powerOn()
{
  startLCD();
  initialLCDState();
}

void power()
{
  IO=!IO;
  if (IO == HIGH)
    powerOn();
  else if (IO == LOW)
    powerOff();
}

void powerOff()
{
  flashLED(4, 10, 20);
  resetVariables();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("Shutting down..."));
  delay(100);
  flashLED(6, 10, 25);
  lcd.noDisplay();
  lcd.setBacklight(LOW);
}


*/
