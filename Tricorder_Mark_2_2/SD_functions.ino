void logData()
{
  logfile = SD.open(filename, FILE_WRITE); 
  logfile.print(gps.time.hour());
  logfile.print(F(":"));
  logfile.print(gps.time.minute());
  logfile.print(F(":"));
  logfile.print(gps.time.second());
  logfile.print(F(":"));
  logfile.print(gps.time.centisecond());
  logfile.print(F(","));
  if (!sType())
  logfile.print(F("Spider"));
  else
  logfile.print(F("Artificial"));
  logfile.print(F(","));
  logfile.print(soilT);
  logfile.print(F(","));
  logfile.print(soilH);
  logfile.print(F(","));
  logfile.print(airT);
  logfile.print(F(","));
  logfile.print(airH);
  logfile.print(F(","));
  logfile.print(full);
  logfile.print(F(","));
  logfile.print(ir);
  logfile.print(F(","));
  logfile.print(full-ir);
  logfile.print(F(","));
  logfile.print(readUV());
  logfile.print(F(","));
  logfile.print(tsl.calculateLux(full, ir));
  logfile.print(F(","));
  logfile.println(readVcc());
  
}

void makeFile()
{
  String name = String(gps.date.year()) + String(gps.date.month()) + String(gps.date.day()) + ".csv";
  name.toCharArray(filename, 13);
  if (!SD.exists(filename))
  {
    logfile = SD.open(filename, FILE_WRITE); 
    logfile.println(F("Time,Sample Type,Soil Temperature,Soil Moisture,Air Temperature,Air Humidity,Full spectrum,IR,Visible,UV,Lux,vcc"));
  }
}

