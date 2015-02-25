
void lcdInitiate()
{
  debug.println(F("Starting Tricorder"));
  //lcd.home();
  lcd.setCursor (3,0);
  lcd.print(F("The Tricorder")); 
  lcd.setCursor (6,1);        // go to start of 2nd line
  lcd.print(F("Mark 2.2"));
  lcd.setCursor (1,2);
  lcd.print(F("The University of"));
  lcd.setCursor (5,3);
  lcd.print(F("Queensland"));
  delay(10000);
  lcd.clear();
}

void startLCD()
{
  lcd.begin (20,4);                         // for 20 x 4 LCD module
  lcd.setBacklightPin(3,POSITIVE);
  lcd.setBacklight(HIGH);
  delay(50);
  debug.println(F("Starting LCD"));
  lcdInitiate();
}

