//SD Logging shield

  void getFileName()
  {
    DateTime now = RTC.now();
    
    filename[0] = (now.year()/1000)%10 + '0'; //To get 1st digit from year()
    filename[1] = (now.year()/100)%10 + '0'; //To get 2nd digit from year()
    filename[2] = (now.year()/10)%10 + '0'; //To get 3rd digit from year()
    filename[3] = now.year()%10 + '0'; //To get 4th digit from year()
    filename[4] = now.month()/10 + '0'; //To get 1st digit from month()
    filename[5] = now.month()%10 + '0'; //To get 2nd digit from month()
    filename[6] = now.day()/10 + '0'; //To get 1st digit from day()
    filename[7] = now.day()%10 + '0'; //To get 2nd digit from day()
    Serial.println (filename);
  }
  
  
  void logData()
    {
      logfile = SD.open(filename, FILE_WRITE);
      #if DEBUG
      Serial.print(filename);
      Serial.println(" is open");
      #endif
      logfile.print(hour);
      logfile.print(":");
      logfile.print(minute);
      logfile.print(":");
      logfile.print(second);
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
      logfile.close();
      #if DEBUG
      Serial.print(filename);
      Serial.println(" is closed");
      #endif
      
      // Now we write data to disk! Don't sync too often - requires 2048 bytes of I/O to SD card
      // which uses a bunch of power and takes time
      if ((millis() - syncTime) < SYNC_INTERVAL) return;
      syncTime = millis();
    }
