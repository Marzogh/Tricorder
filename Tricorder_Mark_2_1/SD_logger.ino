
//SD Logger

void startSD()
{
  Serial.print("Initializing SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(10, OUTPUT);
  
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    error("Card failed, or not present");
  }
  Serial.println("card initialized.");
}


void makeFile()
{
  DateTime now = RTC.now();
    
    /*filename[0] = (now.year()/1000)%10 + '0'; //To get 1st digit from year()
    filename[1] = (now.year()/100)%10 + '0'; //To get 2nd digit from year()
    filename[2] = (now.year()/10)%10 + '0'; //To get 3rd digit from year()
    filename[3] = now.year()%10 + '0'; //To get 4th digit from year()
    filename[4] = now.month()/10 + '0'; //To get 1st digit from month()
    filename[5] = now.month()%10 + '0'; //To get 2nd digit from month()
    filename[6] = now.day()/10 + '0'; //To get 1st digit from day()
    filename[7] = now.day()%10 + '0'; //To get 2nd digit from day()*/
    
    String name = String(now.year()) + String(now.month()) + String(now.day()) + ".csv";
    name.toCharArray(filename, 13);
    logfile = SD.open(filename, FILE_WRITE);
    
    /*if (! SD.exists(filename)) 
    {
      // only open a new file if it doesn't exist
      logfile = SD.open(filename, FILE_WRITE);
    }*/
    
    if (! logfile) 
    {
      error("couldnt create file");
    }
    
    Serial.print("Logging to: ");
    Serial.println(filename);
    
    logfile.println("Time,Soil Temperature,Soil Moisture,Air Temperature,Air Humidity,Full spectrum,IR,Visible,Lux,vcc");    
  #if ECHO_TO_SERIAL
    Serial.println("Time,Soil Temperature,Soil Moisture,Air Temperature,Air Humidity,Full spectrum,IR,Visible,Lux,vcc");
  #endif //ECHO_TO_SERIAL
}

void openFile()
{    
    logfile = SD.open(filename, FILE_WRITE);
        
    if (! logfile) 
    {
      error("couldnt open file");
    }
    
    Serial.print("Logging to: ");
    Serial.println(filename);
}

void logData()
{
  DateTime now = RTC.now();
  
  logfile.print(now.hour(), DEC);
  logfile.print(":");
  logfile.print(now.minute(), DEC);
  logfile.print(":");
  logfile.print(now.second(), DEC);
  logfile.print('"');
  logfile.print(", ");
  if (sType == 1)
  {
    logfile.print("Spider");
  }
  else
  {
    logfile.print("Artificial");
  }
  logfile.print(",");
  logfile.print(soilT);
  logfile.print(", ");    
  logfile.print(soilH);
  logfile.print(", ");
  logfile.print(airT);
  logfile.print(", ");
  logfile.print(airH);
  logfile.print(", ");
  logfile.print(full);
  logfile.print(", ");
  logfile.print(ir);
  logfile.print(", ");
  logfile.print(vis);
  logfile.print(", ");
  logfile.print(lux);
  logfile.println();
  
   // Now we write data to disk! Don't sync too often - requires 2048 bytes of I/O to SD card
  // which uses a bunch of power and takes time
  if ((millis() - syncTime) < SYNC_INTERVAL) return;
  syncTime = millis();
  logfile.close();        //Close file
  #if DEBUG
  Serial.print(filename);
  Serial.println(" is closed");
  #endif
}
