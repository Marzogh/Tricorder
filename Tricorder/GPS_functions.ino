#if GPSattached                                                     //Function is activated only when GPS is attached
boolean startGPS()                                                  //Function starts GPS and checks to see if it is functioning ok.
{
  while (GPS.available() > 0)
  {
    if (gps.encode(GPS.read()))                                     //Reads the NMEA string from the GPS
    {
      #if DEBUG
      displayGPSInfo();
      #endif
      return true;
    }
  }

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    debug.println(F("No GPS detected: check wiring."));
    return false;
  }
}



void displayGPSInfo()                                                //Reads and parses the NMEA string from GPS and outputs location/date/time
{
  Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F("  Date/Time: "));
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" "));
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.print(gps.time.centisecond());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.println();
}
#endif
