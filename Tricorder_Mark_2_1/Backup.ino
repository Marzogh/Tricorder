/*
//Libraries
#include <Wire.h>
#include <Adafruit_HTU21DF.h>
#include <Adafruit_TSL2591.h>
#include <Adafruit_Sensor.h>
#include <SD.h>
#include <RTClib.h>
#include <genieArduino.h>
#include <SoftwareSerial.h>

//Definitions
#define ECHO_TO_SERIAL 1 //For debugging
#define SENSOR_DETAILS 0 //For debugging
#define DS1307_ADDRESS 0x68
#define SYNC_INTERVAL 1000 


//Variables
float airT, airH, airT_avg, airH_avg;
float airT_prev1, airH_prev1 = 1;
float airT_prev2, airH_prev2 = 0;
uint16_t ir, full, vis, lux;
uint16_t readcount = 0;
uint32_t syncTime = 0;
uint32_t totalreadcount = 0;
int  second, minute, hour, weekDay, monthDay, month, year;
uint8_t * heapptr, * stackptr;
char filename[] = "00000000.csv";
int sType = 0;
bool logLED = 0;

//Pins
const int chipSelect = 10; //11, 12, 13 along with 10 previously are used by SD shield for SPI interface
const int powerPin = 5;
const int LCDResetPin = 4;
#define dataPin 9
#define clockPin 8


//Initiate objects
Genie genie;
Adafruit_HTU21DF htu = Adafruit_HTU21DF();
Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591);
RTC_DS1307 RTC;
File logfile;
SoftwareSerial LCDSerial(2, 3); //RX, TX


void setup()
{
  Serial.begin(9600);
  LCDSerial.begin(9600);
  Wire.begin();
  pinMode (LCDResetPin, OUTPUT);
  
  //LCD
  genie.Begin(LCDSerial);   // Use SoftwareSerial for talking to the Genie Library, and to the 4D Systems display
  genie.AttachEventHandler(LCDEventHandler); // Attach the user function Event Handler for processing events
  LCDBegin();
  delay(20);
  
  //HTU21DF
  if(!htu.begin())
  Serial.print("Cannot initiate HTU21DF");
  delay(10);
  
  
  //TSL2591
  if(!tsl.begin())
  Serial.print("Cannot initiate TSL2591");
  delay(10);
  
  
  //SD Logging shield
  #if ECHO_TO_SERIAL
  Serial.print("Initializing SD card...");  
  #endif
  pinMode(chipSelect, OUTPUT);
  if (!SD.begin(chipSelect)) 
  {
    Serial.print("Card failed, or not present");
  }
  #if ECHO_TO_SERIAL
  Serial.println("card initialized");  
  #endif
  getFileName();
  getClock();
}

void loop()
{
  
  readcount++;
 // totalreadcount++;
 
  readSensors();
  writeSensors();
  genie.DoEvents();
  stabilize();
  airH_prev1 = airH;
  airT_prev1 = airT;
 /* //Argument to log data
  if (!readcount == 20 || !readcount == 40)
  { 
   
      //airH_avg += (int) airH;
      //airT_avg += (int) airT; 
      
      #if ECHO_TO_SERIAL
      Serial.print("readcount : ");
      Serial.println(readcount);
      Serial.print("totalreadcount : ");
      Serial.println(totalreadcount);
      #endif
    
  }
  else if (readcount == 20)
  {
    airH_avg1 = airH;
    airT_avg1 = airT;
    airH_avg, airT_avg = 0;
    
    #if ECHO_TO_SERIAL
    Serial.print("readcount : ");
    Serial.println(readcount);
    Serial.print("totalreadcount : ");
    Serial.println(totalreadcount);
    #endif
  }
  else if (readcount == 40)
  {
    airH_avg2 = airH;
    airT_avg2 = airT;
    readcount = 0;
    stabilize();
    
    #if ECHO_TO_SERIAL
    Serial.print("readcount : ");
    Serial.println(readcount);
    Serial.print("totalreadcount : ");
    Serial.println(totalreadcount);
    #endif
  }
   
  #if ECHO_TO_SERIAL
    Serial.print("[ "); Serial.print(millis()); Serial.print(" ms ] ");
    Serial.print("Air Humidity : ");
    Serial.print(airH);
    Serial.print("\t\t");
    Serial.print("Air Temp : ");
    Serial.println(airT);
    delay(20);
    #endif
  delay(10);
  
  check_mem();
  Serial.println(stackptr-heapptr);
}

//Other functions
void stabilize()
{
  if ((!isEqual(airH_prev1, airH) && !isEqual(airT_prev1, airT)) || (!totalreadcount > 200))
  {
    Serial.println("Stabilizing....");
    #if ECHO_TO_SERIAL
    Serial.print("averageH :  ");
    Serial.print(airH_prev1);
    Serial.print(", ");
    Serial.println(airH_prev2);
    Serial.print("averageT :  ");
    Serial.print(airT_prev1);
    Serial.print(", ");
    Serial.println(airT_prev2);
    Serial.print("readcount : ");
    Serial.println(readcount);
    //Serial.print("totalreadcount : ");
    //Serial.println(totalreadcount);
    #endif
  }
  else  
  {
    logLED=1;
    bool UserLed0_val;
    UserLed0_val = logLED;
    genie.WriteObject(GENIE_OBJ_USER_LED, 0x00, UserLed0_val);    // Write UserLed0_val value back to to UserLed0
    #if ECHO_TO_SERIAL
    Serial.print("readcount : ");
    Serial.println(readcount);
    //Serial.print("totalreadcount : ");
    //Serial.println(totalreadcount);
    #endif
    //totalreadcount = 0;
  }
}


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

 
void check_mem() //Function to check RAM When needed Serial.print stackptr-heapptr. It should be > 0
{
  stackptr = (uint8_t *)malloc(4);          // use stackptr temporarily
  heapptr = stackptr;                     // save value of heap pointer
  free(stackptr);      // free up the memory again (sets stackptr to 0)
  stackptr =  (uint8_t *)(SP);           // save value of stack pointer
}


void readSensors()
{
      //HTU21DF
    airH = htu.readHumidity();
    airT = htu.readTemperature();

    #if ECHO_TO_SERIAL
    Serial.print("[ "); Serial.print(millis()); Serial.print(" ms ] ");
    Serial.print("Air Humidity : ");
    Serial.print(airH);
    Serial.print("\t\t");
    Serial.print("Air Temp : ");
    Serial.println(airT);
    delay(20);
    #endif
    
    //TSL2591
    uint32_t lum = tsl.getFullLuminosity();
    ir = lum >> 16;
    full = lum & 0xFFFF;
    vis = full - ir;
    lux = tsl.calculateLux(full, ir);
    
    
    #if SENSOR_DETAILS
    displayTSL2591Details();
    
    #else if ECHO_TO_SERIAL
    Serial.print("[ "); Serial.print(millis()); Serial.print(" ms ] ");
    Serial.print("IR: "); Serial.print(ir);  Serial.print("  ");
    Serial.print("Full: "); Serial.print(full); Serial.print("  ");
    Serial.print("Visible: "); Serial.print(full - ir); Serial.print("  ");
    Serial.print("Lux: "); Serial.println(lux);
    delay (100);
    #endif
}
void error(char *str)
{
  Serial.print("error: ");
  Serial.println(str);
  
  // red LED indicates error
  digitalWrite(redLEDpin, HIGH);

  while(1);
}


//TSL2591
void displayTSL2591Details()
  {
    sensor_t sensor;
    tsl.getSensor(&sensor);
    Serial.println("------------------------------------");
    Serial.print  ("Sensor:       "); Serial.println(sensor.name);
    Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
    Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
    Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" lux");
    Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" lux");
    Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" lux");  
    Serial.println("------------------------------------");
    Serial.println("");
    delay(500);
  }
  
   void configureSensor(void)
  {
    // You can change the gain on the fly, to adapt to brighter/dimmer light situations
    //tsl.setGain(TSL2591_GAIN_LOW);    // 1x gain (bright light)
    tsl.setGain(TSL2591_GAIN_MED);      // 25x gain
    //tsl.setGain(TSL2591_GAIN_HIGH);   // 428x gain
    
    // Changing the integration time gives you a longer time over which to sense light
    // longer timelines are slower, but are good in very low light situtations!
    tsl.setTiming(TSL2591_INTEGRATIONTIME_100MS);  // shortest integration time (bright light)
    //tsl.setTiming(TSL2591_INTEGRATIONTIME_200MS);
    //tsl.setTiming(TSL2591_INTEGRATIONTIME_300MS);
    //tsl.setTiming(TSL2591_INTEGRATIONTIME_400MS);
    //tsl.setTiming(TSL2591_INTEGRATIONTIME_500MS);
    //tsl.setTiming(TSL2591_INTEGRATIONTIME_600MS);  // longest integration time (dim light)
  
  #if ECHO_TO_SERIAL
    // Display the gain and integration time for debugging //  
    Serial.println("------------------------------------");
    Serial.print  ("Gain:         ");
    tsl2591Gain_t gain = tsl.getGain();
    switch(gain)
    {
      case TSL2591_GAIN_LOW:
        Serial.println("1x (Low)");
        break;
      case TSL2591_GAIN_MED:
        Serial.println("25x (Medium)");
        break;
      case TSL2591_GAIN_HIGH:
        Serial.println("428x (High)");
        break;
      case TSL2591_GAIN_MAX:
        Serial.println("9876x (Max)");
        break;
    }
    Serial.print  ("Timing:       ");
    Serial.print((tsl.getTiming() + 1) * 100, DEC); 
    Serial.println(" ms");
    Serial.println("------------------------------------");
    Serial.println("");
    #endif
  }
  
  
  //SD Logging shield

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
    #if ECHO_TO_SERIAL
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
      #if ECHO_TO_SERIAL
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
      #if ECHO_TO_SERIAL
      Serial.print(filename);
      Serial.println(" is closed");
      #endif
      
      // Now we write data to disk! Don't sync too often - requires 2048 bytes of I/O to SD card
      // which uses a bunch of power and takes time
      if ((millis() - syncTime) < SYNC_INTERVAL) return;
      syncTime = millis();
    }
    
    
    //LCD Display
    
   // COMPACT VERSION HERE, EXAMPLE LONGHAND VERSION in display functions MAY BE MORE HUMAN READABLE
  void LCDEventHandler(void)
  {
    genieFrame Event;
  
    genie.DequeueEvent(&Event);
  
    //Read from 4DButton0 and if changed, save data on sample choice
    
    if (genie.EventIs(&Event, GENIE_REPORT_EVENT, GENIE_OBJ_4DBUTTON, 0))
    {
      bool spiderHole = genie.GetEventData(&Event);  // Receive the event data from the 4DButton0
      if (spiderHole == 1)
      {
        sType = 1;
      }
      genie.WriteObject(GENIE_OBJ_FORM, 0x02, 0);
    } 
    
    //Read from 4DButton1 and if changed, save data on sample choice
    if (genie.EventIs(&Event, GENIE_REPORT_EVENT, GENIE_OBJ_4DBUTTON, 1))
    {
      bool artificialHole = genie.GetEventData(&Event);  // Receive the event data from the 4DButton1
      if (artificialHole)
      {
        sType = 2;
      }
      genie.WriteObject(GENIE_OBJ_FORM, 0x02, 0);
    }
    
    //Read from 4DButton2 and if changed, check if UserLED0 is Green - then save data
    if (genie.EventIs(&Event, GENIE_REPORT_EVENT, GENIE_OBJ_4DBUTTON, 2))
    {
      int logValues = genie.GetEventData(&Event);  // Receive the event data from the 4DButton2
      if (!logValues == 0 && !logLED == 0)
      {
        logData();
        genie.WriteObject(GENIE_OBJ_FORM, 0x05, 0);
      }
    }
    
    //Read from 4DButton4 and if 0, cut power
    if (genie.EventIs(&Event, GENIE_REPORT_EVENT, GENIE_OBJ_4DBUTTON, 4))
    {
      int turnOff = genie.GetEventData(&Event);  // Receive the event data from the 4DButton2
      if (!turnOff == 0x01)
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
        pinMode(LCDResetPin, OUTPUT);  // Set D4 on Arduino to Output (4D Arduino Adaptor V2 - Display Reset)
        digitalWrite(LCDResetPin, 1);  // Reset the Display via D4
        delay(100);
        digitalWrite(LCDResetPin, 0);  // unReset the Display via D4
      }
    }
    
   /*  //If the cmd received is from a Reported Object, which occurs if a Read Object (genie.ReadOject) is requested in the main code, reply processed here.
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
    // Reset the Display (change D4 to D2 if you have original 4D Arduino Adaptor)
    // THIS IS IMPORTANT AND CAN PREVENT OUT OF SYNC ISSUES, SLOW SPEED RESPONSE ETC
    // If NOT using a 4D Arduino Adaptor, digitalWrites must be reversed as Display Reset is Active Low, and
    // the 4D Arduino Adaptors invert this signal so must be Active High.  
    pinMode(LCDResetPin, OUTPUT);  // Set D4 on Arduino to Output (4D Arduino Adaptor V2 - Display Reset)
    digitalWrite(LCDResetPin, 1);  // Reset the Display via D4
    delay(100);
    digitalWrite(LCDResetPin, 0);  // unReset the Display via D4
  
    delay (3500); //let the display start up after the reset (This is important)
  
    //Turn the Display on (Contrast) - (Not needed but illustrates how)
    genie.WriteContrast(1); // 1 = Display ON, 0 = Display OFF.
    //For uLCD43, uLCD-70DT, and uLCD-35DT, use 0-15 for Brightness Control, where 0 = Display OFF, though to 15 = Max Brightness ON.
  
    //Write a string to the Display to show the version of the library used
    //genie.WriteStr(0, GENIE_VERSION);
  }
  
  
   /* void writeSoil()
    {
      genie.WriteObject(GENIE_OBJ_THERMOMETER, 0x00, soilT);
      genie.WriteObject(GENIE_OBJ_METER, 0x04, soilH);
    } 
    
      void writeSensors()
    {
      //genie.WriteObject(GENIE_OBJ_THERMOMETER, 0x00, soilT);
      //genie.WriteObject(GENIE_OBJ_METER, 0x04, soilH);
      genie.WriteObject(GENIE_OBJ_THERMOMETER, 1, int (airT));
      genie.WriteObject(GENIE_OBJ_METER, 1, int (airH));
      genie.WriteObject(GENIE_OBJ_LED_DIGITS, 0x00, lux);
      genie.WriteObject(GENIE_OBJ_LED_DIGITS, 0x01, vis);
      genie.WriteObject(GENIE_OBJ_LED_DIGITS, 0x02, ir);
      
    }
    
      /*void writeLight()
    {
      genie.WriteObject(GENIE_OBJ_LED_DIGITS, 0x00, lux);
      genie.WriteObject(GENIE_OBJ_METER, 0x04, vis);
      genie.WriteObject(GENIE_OBJ_METER, 0x05, ir);
    } */
