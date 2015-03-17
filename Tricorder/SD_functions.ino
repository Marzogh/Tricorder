void logData()
{
  debug.println(F("Logging data..."));
  logfile.open(filename, FILE_WRITE); 
  debug.println(F("File open"));
  #if GPSattached
  logfile.print(gps.time.hour());
  logfile.print(F(":"));
  logfile.print(gps.time.minute());
  logfile.print(F(":"));
  logfile.print(gps.time.second());
  logfile.print(F(":"));
  logfile.print(gps.time.centisecond());
  #else
  DateTime now = rtc.now();
  logfile.print(now.hour());
  logfile.print(F(":"));
  logfile.print(now.minute());
  logfile.print(F(":"));
  logfile.print(now.second());
  #endif
  
  logfile.print(F(","));
  if (sType == LOW)
  logfile.print(F("Spider"));
  else
  logfile.print(F("Artificial"));
  logfile.print(F(","));
  logfile.print(soilT,1);
  logfile.print(F(","));
  logfile.print(soilH,1);
  logfile.print(F(","));
  logfile.print(airT,1);
  logfile.print(F(","));
  logfile.print(airH,1);
  logfile.print(F(","));
  logfile.print(full);
  logfile.print(F(","));
  logfile.print(ir);
  logfile.print(F(","));
  logfile.print(full-ir);
  logfile.print(F(","));
  
  #if UVSensor
  logfile.print(readUV());
  logfile.print(F(","));
  #endif
  
  logfile.print(tsl.calculateLux(full, ir));
  logfile.print(F(","));
  logfile.println(readVcc());
  logfile.close();
  
  
  #if GPSattached
  debug.print(gps.time.hour());
  debug.print(F(":"));
  debug.print(gps.time.minute());
  debug.print(F(":"));
  debug.print(gps.time.second());
  debug.print(F(":"));
  debug.print(gps.time.centisecond());
  #else
  debug.print(now.hour());
  debug.print(F(":"));
  debug.print(now.minute());
  debug.print(F(":"));
  debug.print(now.second());
  #endif
  
  debug.print(F(","));
  if (sType == LOW)
  debug.print(F("Spider"));
  else
  debug.print(F("Artificial"));
  debug.print(F(","));
  debug.print(soilT,2);
  debug.print(F(","));
  debug.print(soilH,2);
  debug.print(F(","));
  debug.print(airT,2);
  debug.print(F(","));
  debug.print(airH,2);
  debug.print(F(","));
  debug.print(full);
  debug.print(F(","));
  debug.print(ir);
  debug.print(F(","));
  debug.print(full-ir);
  debug.print(F(","));
  
  #if UVSensor
  debug.print(readUV());
  debug.print(F(","));
  #endif
  
  debug.print(tsl.calculateLux(full, ir));
  debug.print(F(","));
  debug.println(readVcc());
  debug.println(F("Data logged!"));
  blinkSuccess(10);
  
}

void makeFile()
{
  #if GPSattached
  String name = String(gps.date.year()) + String(gps.date.month()) + String(gps.date.day()) + ".csv";
  #else
  DateTime now = rtc.now();
  String name = String(now.year(), DEC) + String(now.month(), DEC) + String(now.day(), DEC) + ".csv";
  #endif
  
  name.toCharArray(filename, 13);
  while (!sd.exists(filename))
  {
    logfile.open(filename, FILE_WRITE); 
    logfile.print(F("Time,Sample Type,Soil Temperature,Soil Moisture,Air Temperature,Air Humidity,Full spectrum,IR,Visible,"));
    
    #if UVSensor
    logfile.print(F("UV,"));
    #endif
    
    logfile.println(F("Lux,vcc"));
    logfile.close();
  }
  blinkSuccess(2);
}

