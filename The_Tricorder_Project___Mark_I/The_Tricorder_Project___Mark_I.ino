  /*
    # Code for Soil Moisture & Temperature sensor + Ambient Light sensor with Nokia 5110 LCD display
    # A.K.A The Tricorder Project - Mark 1
    # Editor     : Prajwal Bhattaram
    # Date       : 29.05.2014
    # Version    : 2.1
  
  */
  #include "LCD_functions.h"                                                                                        
  #include "Sensor_functions.h"
  #include "Sleep_functions.h"
  
  #include <pcd8544.h>
  
  #include <Wire.h>
  #include <SFE_TSL2561.h>
  
  #include <SHT1x.h>
  //#include <util/delay.h>
  //#include <stdlib.h>
  //int seconds=0;
  const int BUTTON=2;
  boolean lastButton = LOW;
  boolean currentButton = LOW;
  
  void setup()
  {
      LCDInitialise();  // Always call lcd.begin() first.
      prepLCD();
      screenBoot();
      Serial.begin(9600);
      //checkBatt();
      pinMode (2, INPUT);
      //digitalWrite (2, LOW);
      prepSensors();
  }
  
  void loop()
  {
      readSample();
       
       delay(2000);
       resample(); //Asks if another sample has to be read
       LEDFadeOn();
       
       prepLCD();
       gotoXY(10,2);
       LCDString("Sleeping");
       delay(500);
       LCDClear();
       delay(250);
       
       digitalWrite(LCD_LED, LOW);
        
       delay(250);
       
           Serial.println("Entering sleep");
           delay(200);
           //seconds = 0;
           sleepNow();
           
           analogWrite(LCD_LED, 100);
           prepLCD();
           delay(50);
        
  
  }
  
  
  
          
