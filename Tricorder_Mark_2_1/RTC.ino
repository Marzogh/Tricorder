

  void getClock()
  {
    // Reset the register pointer
    Wire.beginTransmission(DS1307_ADDRESS);
    byte zero = 0x00;
    Wire.write(zero);
    Wire.endTransmission();
    Wire.requestFrom(DS1307_ADDRESS, 7);
    
    second = bcdToDec(Wire.read());
    minute = bcdToDec(Wire.read());
    hour = bcdToDec(Wire.read() & 0b111111); //24 hour time
    weekDay = bcdToDec(Wire.read()); //0-6 -> sunday - Saturday
    monthDay = bcdToDec(Wire.read());
    month = bcdToDec(Wire.read());
    year = bcdToDec(Wire.read());
    #if DEBUG
    Serial.print (year);
    Serial.print("/");
    Serial.print(month);
    Serial.print("/");
    Serial.print(monthDay);
    Serial.print(" ");
    Serial.print(hour);
    Serial.print(":");
    Serial.print(minute);
    Serial.print(":");
    Serial.println(second);
    #endif
  }
