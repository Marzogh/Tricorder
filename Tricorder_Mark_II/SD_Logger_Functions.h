/*
RTC_DS1307 RTC; // define the Real Time Clock object

// for the data logging shield, we use digital pin 10 for the SD cs line
const int chipSelect = 10;

// the digital pins that connect to the LEDs
#define redLEDpin 2
#define greenLEDpin 3

#define SYNC_INTERVAL 1000 // mills between calls to flush() - to write data to the card
uint32_t syncTime = 0; // time of last sync()

// Initiate an instance of the logging file
File logfile;

void error(char *str)
{
  Serial.print("error: ");
  Serial.println(str);
  
  // red LED indicates error
  digitalWrite(redLEDpin, HIGH);

  while(1);
}


void SDInitialize()
    {
     // initialize the SD card
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

void makeLogFile()
    {
     DateTime now = RTC.now();
    // Code to create a dynamic file name on the SD card 
    
     /*String logfileName = String(String(sedClock.getTime().year(),DEC) + 
                             String(sedClock.getTime().month(),DEC) + 
                             String(sedClock.getTime().day(),DEC) + 
                             String(sedClock.getTime().hour(),DEC) + 
                             String(sedClock.getTime().minute(),DEC) + 
                             String(sedClock.getTime().second(),DEC) + '_log.txt');*/
      /*String logfileName = String(String(now.year(),DEC) + 
                             String(now.month(),DEC) + 
                             String(now.day(),DEC) + '_log.csv'); 
                             //String(now.hour(),DEC) + 
                             //String(now.minute(),DEC) + 
                             //String(now.second(),DEC) + '_log.csv');
     
      char __logfileName[sizeof(logfileName)];
      logfileName.toCharArray(__logfileName, sizeof(__logfileName));
      pinMode(chipSelect,OUTPUT);
      logfile = SD.open(__logfileName,FILE_WRITE);
    }
    
void logData()
    {
     DateTime now = RTC.now();
      // log time
      //logfile.print(now.unixtime()); // seconds since 1/1/1970
      //logfile.print(", ");
      //logfile.print('"');
      //logfile.print(now.year(), DEC);
      //logfile.print("/");
      //logfile.print(now.month(), DEC);
      //logfile.print("/");
      //logfile.print(now.day(), DEC);
      //logfile.print(" ");
      logfile.print(now.hour(), DEC);
      logfile.print(":");
      logfile.print(now.minute(), DEC);
      logfile.print(":");
      logfile.print(now.second(), DEC);
      logfile.print('"');
      logfile.print(", ");
      /*if (sType == 1)
      {
        logfile.print("Spider");
      }
      else
      {
        logfile.print("Artificial");
      }*/
      /*logfile.print(",");
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
      
      // blink LED to show we are syncing data to the card & updating FAT!
      digitalWrite(redLEDpin, HIGH);
      logfile.flush();
      digitalWrite(redLEDpin, LOW);
    }*/
