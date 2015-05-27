/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~ SD Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void makeFile()
{
  DateTime now = rtc.now();
  String name = String(now.year(), DEC) + String(now.month(), DEC) + String(now.day(), DEC) + ".csv";

  name.toCharArray(filename, 13);
  while (!sd.exists(filename))
  {
    logfile.open(filename, FILE_WRITE);
    logfile.print(F("Time,Sample ID,Soil Temperature,Soil Moisture,Air Temperature,Air Humidity,"));

    logfile.println(F("Lux,vcc"));
    logfile.close();
  }
  blinkSuccess(2);
}

void logData()
{
  Serial.println(F("Logging data..."));
  logfile.open(filename, FILE_WRITE);
  Serial.println(F("File open"));
  DateTime now = rtc.now();
  logfile.print(now.hour());
  logfile.print(F(":"));
  logfile.print(now.minute());
  logfile.print(F(":"));
  logfile.print(now.second());

  logfile.print(F(","));
  switch (sType) {
    case 1:
    logfile.print(F("SL"));
    logfile.print(sNo);
    break;
    case 2:
    logfile.print(F("S"));
    logfile.print(sNo);
    break;
    case 3:
    logfile.print(F("AL"));
    logfile.print(sNo);
    break;
    case 4:
    logfile.print(F("A"));
    logfile.print(sNo);
    break;
  }
  logfile.print(F(","));
  logfile.print(soilT, 1);
  logfile.print(F(","));
  logfile.print(soilH, 1);
  logfile.print(F(","));
  logfile.print(airT, 1);
  logfile.print(F(","));
  logfile.print(airH, 1);
  logfile.print(F(","));
  logfile.print(lux);
  logfile.print(F(","));

  logfile.println(readVcc());
  logfile.close();

  Serial.print(now.hour());
  Serial.print(F(":"));
  Serial.print(now.minute());
  Serial.print(F(":"));
  Serial.print(now.second());

  Serial.print(F(","));
  switch (sType) {
    case 1:
    Serial.print(F("SL"));
    Serial.print(sNo);
    break;
    case 2:
    Serial.print(F("S"));
    Serial.print(sNo);
    break;
    case 3:
    Serial.print(F("AL"));
    Serial.print(sNo);
    break;
    case 4:
    Serial.print(F("A"));
    Serial.print(sNo);
    break;
  }
  Serial.print(F(","));
  Serial.print(soilT, 1);
  Serial.print(F(","));
  Serial.print(soilH, 1);
  Serial.print(F(","));
  Serial.print(airT, 1);
  Serial.print(F(","));
  Serial.print(airH, 1);
  Serial.print(F(","));
  Serial.print(lux);
  Serial.print(F(","));

  Serial.println(readVcc());
  Serial.println(F("Data logged!"));
  blinkSuccess(10);
}
