
void initiateLCD()                                                          //Initiates LCD
{
  debug.println(F("Starting Tricorder"));
  /*Use code below for 20x4 display
  lcd.setCursor (3,0);
  lcd.print(F("The Tricorder")); 
  lcd.setCursor (6,1);                                                     
  lcd.print(F("Mark 2.2"));
  lcd.setCursor (1,2);
  lcd.print(F("The University of"));
  lcd.setCursor (5,3);
  lcd.print(F("Queensland")); */
  /*Use code below for 16x2 display */
  lcd.print(F("The Tricorder")); 
  lcd.setCursor (0,1);                                                     
  lcd.print(F("Uni of Qld"));
  delay(2000);
  lcd.clear();
}

void startLCD()
{
  //lcd.begin (20,4);                                                         //Settings for 20 x 4 LCD module. For the 20x2 module change to (20,2)
  lcd.begin (16,2);                                                         //Settings for 20 x 4 LCD module. For the 20x2 module change to (20,2)
  lcd.setBacklightPin(3,POSITIVE);
  lcd.setBacklight(HIGH);
  delay(50);
  debug.println(F("Starting LCD"));
  initiateLCD();
}

void initialLCDState()
{
  lcd.home();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("Take a reading"));                //LCD shows message
  while (newRead == LOW)
  {
    debug.println(F("Take a reading..."));       //This section on code animates the "Take a reading......" that is displayed on the LCD screen
    lcd.setCursor(14,0);
    uint8_t i=0;
    //while (i<6)                                //For 20x4 display
    while (i<3)                                  //For 16x2 display
    {
    lcd.print(".");
    i++;
    delay(50);
    }
    delay(500);
    lcd.setCursor(14,0);
    lcd.print("      ");
    delay(10);
  }
}

void displaySensors(uint8_t repeats)                                                       //Displays sensor values on LCD after the readings are done
{
  uint8_t i = 0;
  while (i<repeats)
  {
  lcd.clear();
  lcd.setCursor (0,0);
  lcd.print(F("Soil Temp "));
  lcd.print(soilT, 1);
  lcd.print(F(" C"));
  lcd.setCursor (0,1);
  lcd.print(F("Moisture% "));
  lcd.print(soilH, 1);
  lcd.print(F("%"));
  delay(2000);
  
  lcd.clear();
  lcd.setCursor (0,0);
  lcd.print(F("Air Temp "));
  lcd.print(airT, 1);
  lcd.print(F(" C"));
  lcd.setCursor (0,1);
  lcd.print(F("Humidity "));
  lcd.print(airH, 1);
  lcd.print(F("%"));
  delay(2000);
  
  #if TSL2591attached
  lcd.clear();
  lcd.setCursor (0,0);
  lcd.print(F("Visible: "));
  lcd.print(vis);
  lcd.setCursor (0,1);
  lcd.print(F("IR: "));
  lcd.print(ir);
  delay(2000);
  #endif
  
  lcd.clear();
  lcd.setCursor (0,0);
  lcd.print(F("UV: "));
  #if UVSensor
  lcd.print(readUV());
  #else
  lcd.print("-NA-");
  #endif
  lcd.setCursor (0,1);
  lcd.print(F("Lux: "));
  lcd.print(lux);
  //lcd.print(F(" lux"));
  delay(2000);
  
  i++;
  }
}

