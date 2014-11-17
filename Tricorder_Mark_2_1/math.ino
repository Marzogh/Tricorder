
boolean isEqual(float f1, float f2) //Makes it easy to compare floats up to any number of decimal places required
{
  return ( (int)(f1 *10)) == ((int)(f2 * 10) );
   #if ECHO_TO_SERIAL
    Serial.print("avg1 : ");
    Serial.println((int)(f1 *10));
    Serial.print("avg2 : ");
    Serial.println((int)(f2 *10));
    #endif
}


byte bcdToDec(byte val)
{
  // Convert binary coded decimal to normal decimal bers
  return ( (val/16*10) + (val%16) );
}

void error(char *str)
{
  Serial.print("error: ");
  Serial.println(str);
  
  // red LED indicates error
  digitalWrite(redLEDpin, HIGH);
  
  while(1);
}
