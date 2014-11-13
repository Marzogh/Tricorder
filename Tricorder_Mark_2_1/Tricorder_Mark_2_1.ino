  
  //Libraries
  #include <Wire.h>
  #include <Adafruit_TSL2591.h>
  #include <Adafruit_Sensor.h>
  #include <Adafruit_HTU21DF.h>
  #include <SHT1x.h>
  #include <SD.h>
  #include <RTClib.h>
  
  
  //Definitions
  #define DEBUG true
  #define DS1307_ADDRESS 0x68
  #define SYNC_INTERVAL 1000 
  
  
  //Variables
  int ir, vis, full, lux;
  float airT, airH, soilT, soilH;
  //float temp_f; //Uncomment for temperature in Fahrenheit
  uint32_t syncTime, sType = 0;
  int  second, minute, hour, weekDay, monthDay, month, year;
  char filename[] = "00000000.csv";
  uint8_t * heapptr, * stackptr;
  
  
  //Pins
  #define dataPin  9
  #define clockPin 8
  
  
  //Initiate Objects
  Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591);
  Adafruit_HTU21DF htu = Adafruit_HTU21DF();
  SHT1x sht1x(dataPin, clockPin);
  RTC_DS1307 RTC;
  File logfile;
  
  void setup()
  {
    Serial.begin(115200);
    startTSL();
    startHTU();
    
    #if DEBUG
    displayTSLDetails();
    #endif
  }
  
  
  void loop()
  {
    readTSL();
    readHTU();
    readSHT();
    void getClock();
    void getFileName();
    void logData();
    
  }
