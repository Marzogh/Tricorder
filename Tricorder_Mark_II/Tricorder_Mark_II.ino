  //Include all libraries needed by the sensor system
  
  #include <genieArduino.h>
  #include <SHT1x.h>
  #include <SD.h>
  #include <RTClib.h>
  #include <Adafruit_TSL2591.h>
  #include <SoftwareSerial.h>
  #include <Adafruit_HTU21DF.h>
  #include <Wire.h>
  #include <Adafruit_Sensor.h>
  #include <avr/interrupt.h>
  #include <avr/power.h>
  #include <avr/sleep.h>
  
  //#include "Display_Functions.h"
  #include "SHT1x_Functions.h"
  //#include "HTU21DF_Functions.h"
  #include "TSL2591_Functions.h"
  //#include "SD_Logger_Functions.h"
  #include "Sleep_Functions.h"
  
  //#define RESETLINE 4  // Change this if you are not using an Arduino Adaptor Shield Version 2 (see code below)
  #define SYNC_INTERVAL 1000 // mills between calls to flush() - to write data to the card
  // the digital pins that connect to the LEDs
  #define redLEDpin 2
  #define greenLEDpin 3
  #define powerPin 5
  #define LCDResetPin 4
  #define DS1307_ADDRESS 0x68
  
  Genie genie;
  RTC_DS1307 RTC; // define the Real Time Clock object
  char filename[] = "00000000.csv";
  File logfile; // Initiate an instance of the logging file
  Adafruit_HTU21DF htu = Adafruit_HTU21DF();
  
  float airT, airH;
  const int chipSelect = 10; // for the data logging shield, we use digital pin 10 for the SD cs line
  uint32_t syncTime = 0; // time of last sync()
  int  second, minute, hour, weekDay, monthDay, month, year;
  uint8_t * heapptr, * stackptr;

  int sType;
  bool logLED = 0;

  SoftwareSerial mySerial(7, 8); //RX, TX
    
  void setup()
  {
    Serial.begin(9600);
    //Initialize LCD & Software serial
    mySerial.begin(9600);
    LCDBegin();
    //Initialize I2C
    Wire.begin();
    //Initialize light sensor
    tsl.begin();
    htu.begin();
    //displayTSL2591Details();
    configureSensor();  
    //Initialize logging shield
    if (!RTC.begin())
    {
      Serial.println("RTC failed");
    }
    SDInitialize();
    getClock();
    getFileName();
  }
  
  
  void loop()
  {
    readLight();
    check_mem();
    Serial.println(stackptr - heapptr);
    //delay(250);
    //writeLight();
    readAir();
    //delay(250);
    //writeAir();
    //readSoil();
    //writeSoil();
    //logData();
    //delay(500);
    logfile = SD.open(filename, FILE_WRITE);
      Serial.print(filename);
      Serial.println(" is open");
      logfile.print(hour);
      logfile.print(":");
      logfile.print(minute);
      logfile.print(":");
      logfile.print(second);
      logfile.print('"');
      logfile.print(", ");
     /* if (sType == 1)
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
      logfile.print(", ");*/
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
      
      // Now we write data to disk! Don't sync too often - requires 2048 bytes of I/O to SD card
      // which uses a bunch of power and takes time
      if ((millis() - syncTime) < SYNC_INTERVAL) return;
      syncTime = millis();
    
    
    //Write stabilization code for values and report stabilized values to USERLED0/logLED. Also include timeout function.
  }
  
  
  /*************************************************************************************************************************************************************************************************************/
  
  /*                                                                                Display code                                                                                                               */
  
  /*************************************************************************************************************************************************************************************************************/

// COMPACT VERSION HERE, EXAMPLE LONGHAND VERSION in display finctions MAY BE MORE HUMAN READABLE
void myGenieEventHandler(void)
{
  genieFrame Event;

  genie.DequeueEvent(&Event);

  //Read from 4DButton0 and if changed, save data on sample choice
  if (genie.EventIs(&Event, GENIE_REPORT_EVENT, GENIE_OBJ_4DBUTTON, 0))
  {
    int spiderHole = genie.GetEventData(&Event);  // Receive the event data from the 4DButton0
    if (!spiderHole == 0)
    {
      sType = 1;
    }
  }
  
  //Read from 4DButton1 and if changed, save data on sample choice
  if (genie.EventIs(&Event, GENIE_REPORT_EVENT, GENIE_OBJ_4DBUTTON, 1))
  {
    int artificialHole = genie.GetEventData(&Event);  // Receive the event data from the 4DButton1
    if (!artificialHole == 0)
    {
      sType = 2;
    }
  }
  
  //Read from 4DButton2 and if changed, check if UserLED0 is Green - then save data
  if (genie.EventIs(&Event, GENIE_REPORT_EVENT, GENIE_OBJ_4DBUTTON, 2))
  {
    int logValues = genie.GetEventData(&Event);  // Receive the event data from the 4DButton2
    if (!logValues == 0 && !logLED == 0)
    {
      logData();
      logfile.close();
    }
  }
  
  //Read from 4DButton4 and if 0, cut power
  if (genie.EventIs(&Event, GENIE_REPORT_EVENT, GENIE_OBJ_4DBUTTON, 4))
  {
    int turnOff = genie.GetEventData(&Event);  // Receive the event data from the 4DButton2
    if (!turnOff == 1)
    {
      digitalWrite(powerPin, HIGH);
    }
  }
  
  //Read from WinButton5 and if 1, refresh screen
  if (genie.EventIs(&Event, GENIE_REPORT_EVENT, GENIE_OBJ_WINBUTTON, 5))
  {
    int newReading = genie.GetEventData(&Event);  // Receive the event data from the 4DButton2
    if (!newReading == 0)
    {
      digitalWrite(LCDResetPin, HIGH);
    }
  }
  
   //If the cmd received is from a Reported Object, which occurs if a Read Object (genie.ReadOject) is requested in the main code, reply processed here.
  if (Event.reportObject.cmd == GENIE_REPORT_OBJ && Event.reportObject.object == GENIE_OBJ_USER_LED && Event.reportObject.index == 0)
  {
    bool UserLed0_val = genie.GetEventData(&Event);               // Receive the event data from the UserLed0
    UserLed0_val = logLED;
    genie.WriteObject(GENIE_OBJ_USER_LED, 0x00, UserLed0_val);    // Write UserLed0_val value back to to UserLed0
  }
  
}

//Code to initiate LCD
void LCDBegin()
{
  // NOTE, the genieBegin function (e.g. genieBegin(GENIE_SERIAL_0, 115200)) no longer exists.  Use a Serial Begin and serial port of your choice in
  // your code and use the genie.Begin function to send it to the Genie library (see this example below)
  // 200K Baud is good for most Arduinos. Galileo should use 115200.
  genie.Begin(mySerial);   // Use Serial0 for talking to the Genie Library, and to the 4D Systems display

  genie.AttachEventHandler(myGenieEventHandler); // Attach the user function Event Handler for processing events

  // Reset the Display (change D4 to D2 if you have original 4D Arduino Adaptor)
  // THIS IS IMPORTANT AND CAN PREVENT OUT OF SYNC ISSUES, SLOW SPEED RESPONSE ETC
  // If NOT using a 4D Arduino Adaptor, digitalWrites must be reversed as Display Reset is Active Low, and
  // the 4D Arduino Adaptors invert this signal so must be Active High.  
  pinMode(RESETLINE, OUTPUT);  // Set D4 on Arduino to Output (4D Arduino Adaptor V2 - Display Reset)
  digitalWrite(RESETLINE, 1);  // Reset the Display via D4
  delay(100);
  digitalWrite(RESETLINE, 0);  // unReset the Display via D4

  delay (3500); //let the display start up after the reset (This is important)

  //Turn the Display on (Contrast) - (Not needed but illustrates how)
  genie.WriteContrast(1); // 1 = Display ON, 0 = Display OFF.
  //For uLCD43, uLCD-70DT, and uLCD-35DT, use 0-15 for Brightness Control, where 0 = Display OFF, though to 15 = Max Brightness ON.

  //Write a string to the Display to show the version of the library used
  genie.WriteStr(0, GENIE_VERSION);
}


  void writeSoil()
  {
    genie.WriteObject(GENIE_OBJ_THERMOMETER, 0x00, soilT);
    genie.WriteObject(GENIE_OBJ_METER, 0x04, soilH);
  }
  
    void writeAir()
  {
    genie.WriteObject(GENIE_OBJ_THERMOMETER, 0x00, airT);
    genie.WriteObject(GENIE_OBJ_METER, 0x04, airH);
  }
  
    void writeLight()
  {
    genie.WriteObject(GENIE_OBJ_LED_DIGITS, 0x00, lux);
    genie.WriteObject(GENIE_OBJ_METER, 0x04, vis);
    genie.WriteObject(GENIE_OBJ_METER, 0x05, ir);
  }

/*************************************************************************************************************************************************************************************************************/
  
/*                                                                                SD logger code                                                                                                             */
  
/*************************************************************************************************************************************************************************************************************/

void error(char *str)
{
  Serial.print("error: ");
  Serial.println(str);
  
  // red LED indicates error
  digitalWrite(redLEDpin, HIGH);

  while(1);
}


void getFileName(){

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

void logData()
    {
      logfile = SD.open(filename, FILE_WRITE);
      Serial.print(filename);
      Serial.println(" is open");
      logfile.print(hour);
      logfile.print(":");
      logfile.print(minute);
      logfile.print(":");
      logfile.print(second);
      logfile.print('"');
      logfile.print(", ");
     /* if (sType == 1)
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
      logfile.print(", ");*/
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
      
      // Now we write data to disk! Don't sync too often - requires 2048 bytes of I/O to SD card
      // which uses a bunch of power and takes time
      if ((millis() - syncTime) < SYNC_INTERVAL) return;
      syncTime = millis();
      
      // blink LED to show we are syncing data to the card & updating FAT!
      digitalWrite(redLEDpin, HIGH);
      logfile.flush();
      digitalWrite(redLEDpin, LOW);
    }
    
  void getClock(){
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
    }
    
    byte bcdToDec(byte val)
    {
      // Convert binary coded decimal to normal decimal bers
      return ( (val/16*10) + (val%16) );
    }
    
  void check_mem() 
  {
    stackptr = (uint8_t *)malloc(4);          // use stackptr temporarily
    heapptr = stackptr;                     // save value of heap pointer
    free(stackptr);      // free up the memory again (sets stackptr to 0)
    stackptr =  (uint8_t *)(SP);           // save value of stack pointer
  }

/*************************************************************************************************************************************************************************************************************/
  
/*                                                                                HTU21DF code                                                                                                             */
  
/*************************************************************************************************************************************************************************************************************/

  void readAir()
  {
    airH = htu.readHumidity();
    airT = htu.readTemperature();
    Serial.print("[ "); Serial.print(millis()); Serial.print(" ms ] ");
    Serial.print("Air Humidity : ");
    Serial.print(airH);
    Serial.print("\t\t");
    Serial.print("Air Temp : ");
    Serial.println(airT);
    delay(100);
  }
