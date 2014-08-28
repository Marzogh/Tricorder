  #include <Wire.h>
  #include <SFE_TSL2561.h>
  
  #include <SHT1x.h>
  
  
  //const int TnHSens=0;          //Moisture sensor is on Analog Pin 0
  const int TnHSens_PWR=12;      //Power to the sensor is controlled through Digital Pin 2
  //const int LightSens=3;        //Light Sensor is on Digital Pin 3
  const int LightSens_PWR=7;    //Power to light sensor is controlled through Digital Pin 4
  const int LED=5;
  
  
  //Sensor variables
  int MIN_READ = 0;             //Minimum expected moisture value (For calibration)
  int MAX_READ = 0;             //Maximum Expected moisture value (For calibration)
  int voltage = 0;                  //Voltage reading
  boolean gain;                 // Gain setting, 0 = X1, 1 = X16;
  unsigned int ms;              // Integration ("shutter") time in milliseconds
  
  
  // Specify data and clock connections and instantiate SHT1x object
  #define dataPin  3
  #define clockPin 4
  SHT1x sht1x(dataPin, clockPin);
  
  // Create an SFE_TSL2561 object, here called "light":
  SFE_TSL2561 light;
  
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Sensor Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
  
  /* 
      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~DEBUGGING CODE BEGIN~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
  
  //calibrationfail()
  
            void calibration_fail()
           { 
             prepLCD();
             gotoXY(3,1);
             LCDString("Calibration");
             gotoXY(20,2);
             LCDString ("failed");
             gotoXY(10,4);
             LCDString("Try again");
             Serial.println("Calibration failed. Try again");
           }
           
  
  //Calibrate()
  
  void calibrate()
          {
                prepLCD();
                gotoXY(4,1);
                LCDString("Calibration");
                gotoXY(14,2);
                LCDString("required");             //Turns on display and asks to calibrate sensor
                Serial.println("Calibration required");
                delay(3000);
                
                prepLCD();
                gotoXY(4,2);
                LCDString("0% Humidity");
                digitalWrite(TnHSens_PWR, HIGH); //zero point calibration begins
                Serial.print("0% humidity: ");
                delay(2000);
                
                prepLCD();
                gotoXY(12,1);
                LCDString("Place in");
                gotoXY(32,4);
                LCDString("air");
                
                MIN_READ = analogRead(0);        //Read sensor
                delay(2500);
                digitalWrite(TnHSens, LOW);
                Serial.println(MIN_READ);
                delay(1500);
                
                prepLCD();
                gotoXY(4,1);
                LCDString("First stage");
                gotoXY(14,4);
                LCDString("Complete");
                Serial.println("First stage complete");
                delay(3000);
                
                prepLCD();
                gotoXY(24,1);
                LCDString("100%");
                gotoXY(16,4);
                LCDString("Humidity");
                digitalWrite(TnHSens_PWR, HIGH); //100% saturation calibration begins
                Serial.print("100% humidity: ");
                delay(2000);
                
                prepLCD();
                gotoXY(12,1);
                LCDString("Place in");
                gotoXY(24,4);
                LCDString("water");
                
                MAX_READ = analogRead(0);        //Read sensor
                delay(2500);
                digitalWrite(TnHSens_PWR, LOW);
                Serial.println(MAX_READ);
                delay(1500);
                
                //LCDClear(); 
                //drawBox();
                //gotoXY(4,1);
                //LCDString("");
                //gotoXY(14,4);
                //LCDString("Complete");
                    
               if (MAX_READ == MIN_READ)               //If sensor reading has changed
              {
                
                calibration_fail();
              }
            else
              {
                prepLCD();
                gotoXY(4,1);
                LCDString("Calibration");
                gotoXY(16,4);
                LCDString("complete"); //100% saturation calibration complete
                Serial.println("Calibration complete");
                delay(3000);
              }
          }
  
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~DEBUGGING CODE END~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
  */
  
  //dot()
       
       void dot()
       {
         digitalWrite(LED, HIGH);
         delay(250);
         digitalWrite(LED, LOW);
         delay(250);
       }
  
  
  //dash()
       
       void dash()
       {
         digitalWrite(LED, HIGH);
         delay(500);
         digitalWrite(LED, LOW);
         delay(500);
       }
       
       
  //SOS()
  
       void SOS()
       {
         dot(),dot(),dot();
         delay(500);
         dash(), dash(), dash();
         delay(500);
         dot(), dot(),dot();
         delay(1000);
       }
  
  
  
  //prepLightSensor()
  void prepSensors()
      {
        pinMode (TnHSens_PWR, OUTPUT); //Sets SHT15 sensor power pin to output
        pinMode (LightSens_PWR, OUTPUT); //Sets TSL2561 sensor power pin to output
        //pinMode (LightSens, INPUT);
        //pinMode (TnHSens, INPUT);
        pinMode (LCD_LED, OUTPUT); //Sets display's LED power pin to output
        pinMode (2, INPUT);  //Sets sleep interrupt pin to input
        
        digitalWrite(LightSens_PWR, HIGH);
      
        light.begin(); //Initiates and sets up TSL2561
          
          // The light sensor has a default integration time of 402ms and a default gain of low (1X).
          
          // This can be changed by using the setTiming() command.
          
          // If gain = false (0), device is set to low gain (1X)
          // If gain = high (1), device is set to high gain (16X)
        
          gain = 0;
        
          // If time = 0, integration will be 13.7ms
          // If time = 1, integration will be 101ms
          // If time = 2, integration will be 402ms
          // If time = 3, use manual start / stop to perform your own integration
        
          unsigned char time = 2;
        
          // setTiming() will set the third parameter (ms) to the requested integration time in ms (this will be useful later):
          
          Serial.println("Set timing...");
          light.setTiming(gain,time,ms);
          
           // To start taking measurements, power up the sensor:
  
          Serial.println("Powerup...");
          light.setPowerUp();
  
  
      }
  
  //printError()
  void printError(byte error)
                  // If there's an I2C error, this function will print out an explanation.
                {
                  Serial.print("I2C error: ");
                  Serial.print(error,DEC);
                  Serial.print(", ");
                  
                  switch(error)
                  {
                    case 0:
                      Serial.println("success");
                      break;
                    case 1:
                      Serial.println("data too long for transmit buffer");
                      break;
                    case 2:
                      Serial.println("received NACK on address (disconnected?)");
                      break;
                    case 3:
                      Serial.println("received NACK on data");
                      break;
                    case 4:
                      Serial.println("other error");
                      break;
                    default:
                      Serial.println("unknown error");
                  }
                SOS();
                delay(1500);
                }
  
  //blinkLED()
           void blinkLED()
           {
             digitalWrite(LED, HIGH);
             delay(150);
             digitalWrite(LED, LOW);
           }
           
  //readSample()
  
           void readSample()
           {
                 
                float temp_c;
                float humidity;
                int q = 1;
                
                prepLCD();
                gotoXY(12,1);
                //digitalWrite(TnHSens_PWR,, HIGH);
                LCDString("Place in");
                gotoXY(26,4);
                LCDString("soil");
                
                  digitalWrite(TnHSens_PWR, HIGH);
                  //digitalWrite(LightSens_PWR, HIGH);
                  //delay(20);
                  //light.setPowerUp();
                  delay(150);                        //Wait for sensors to initialise
                  temp_c = sht1x.readTemperatureC();
                  humidity = sht1x.readHumidity();
                  
                  //Future changes: Write code here to double check that the sensor is actually in the soil (Sort of like a debounce code)
                  
                  blinkLED();
                  delay(1000);
                  
                  // There are two light sensors on the device, one for visible light and one for infrared. Both sensors are needed for lux calculations.
                  
                  // Retrieve the data from the device:
                
                  unsigned int data0, data1;
                  double lux;    // Resulting lux value
                  boolean good;  // True if neither sensor is saturated
                  
                  if (light.getData(data0,data1))
                  {
                    // getData() returned true, communication was successful
                    
                    //Serial.print("data0: ");
                    //Serial.print(data0);
                    //Serial.print(" data1: ");
                    //Serial.print(data1);
                  
                    // To calculate lux, pass all your settings and readings to the getLux() function.
                    
                    /* The getLux() function will return 1 if the calculation was successful, or 0 if one or both of the sensors was saturated (too much light).
                    If this happens, you can reduce the integration time and/or gain. */
                    
                    // Perform lux calculation:
                
                    good = light.getLux(gain,ms,data0,data1,lux);
                    
                    // Print out the results:
                	
                    //Serial.print(" lux: ");
                    //Serial.print(lux);
                    //if (good) Serial.println(" (good)"); else Serial.println(" (BAD)");
                    //blinkLED();
                  }
                  else
                  {
                    // getData() returned false because of an I2C error, inform the user.
                
                    byte error = light.getError();
                    printError(error);
                  }
                  
                  delay(300);                          
                 
                  light.setPowerDown();
                  delay(20);
                  digitalWrite(TnHSens_PWR, LOW);
                  digitalWrite(LightSens_PWR, LOW);               
                  //Serial.print("Read ");             //Debugging code begins
                  //Serial.print(q);
                  //Serial.print(" = ");
                  //Serial.println(val);              //Debugging code ends
                  
                  
                  
                
                //val = val / 5;
                
                //val = map(val, MIN_READ, MAX_READ, 0, 100);
                //val = constrain(val, 0, 100);
                
                //int a=1;
                char char_data[5];         
   
                
                //Convert val from string to char              
                //String str;
                //str=String(lux);
                //str.toCharArray(lightRead,5);
                //delay(50);
                //str=String(temp_c);
                //str.toCharArray(temperatureRead,5);
                //delay(50);
                //str=String(humidity);
                //str.toCharArray(humidityRead,5);
                
             Serial.print("Moisture Content: ");
                Serial.print(humidity);
                Serial.println("%");
                Serial.print("Temperature: ");
                Serial.print(temp_c);
                Serial.println("C");
                Serial.print(" Luminosity: ");
                Serial.print(lux);
                Serial.print(" lux");
                if (good) Serial.println(" (good)"); else Serial.println(" (BAD)");
                
            do
              {    
                prepLCD();
                dtostrf(humidity, 5, 1, char_data);    //dtostrf () converts float and double variables to strings
                gotoXY(24,1);
                LCDString("Soil ");
                gotoXY(12,2);
                LCDString("Moisture:");
                gotoXY(12,4);
                LCDString(char_data);
                LCDString(" %");
                delay(3500);
                //prepLCD();
                
                prepLCD();
                dtostrf(temp_c, 5, 1, char_data);
                gotoXY(8,1);
                LCDString("Soil Temp:");
               // gotoXY(12,2);
               // LCDString("Temp =");
                gotoXY(14,4);
                LCDString(char_data);
                LCDString(" C");
                delay(3500);
                //prepLCD();
                
                //if (good)
               // {
                  prepLCD();
                  dtostrf(lux, 5, 1, char_data);
                  gotoXY(16,1);
                  LCDString("Ambient ");
                  gotoXY(20,2);
                  LCDString("Light:");
                  gotoXY(10,4);
                  LCDString(char_data);
                  LCDString(" lux");
                  delay(3500);
                  //prepLCD();
              //  }
           /* else
               {
                  prepLCD();
                  gotoXY(8,1);
                  LCDString("Ambient ");
                  gotoXY(10,2);
                  LCDString("Light =");
                  gotoXY(18,4);
                  //LCDString(lightRead);
                  LCDString(" FAIL");
                  delay(4000);
               }  */  
               
               q++;        
             } while (q<3);
           }
  
 /* Unused function
 //finish()
      void finish()
       {
          prepLCD();
          gotoXY(4,1);
          LCDString("Goodbye!!!");
          
          for (int i=0; i<100; i--)
            {
              analogWrite(LCD_LED, i);
              delay(10);
            }
          
          return;
       }
  */     
  
  //readVCC()
  
       long readVcc() 
       {
          // Read 1.1V reference against AVcc
          // set the reference to Vcc and the measurement to the internal 1.1V reference
          #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
            ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
          #elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
            ADMUX = _BV(MUX5) | _BV(MUX0);
          #elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
            ADMUX = _BV(MUX3) | _BV(MUX2);
          #else
            ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
          #endif  
         
          delay(250); // Wait for Vref to settle
          ADCSRA |= _BV(ADSC); // Start conversion
          while (bit_is_set(ADCSRA,ADSC)); // measuring
         
          uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH  
          uint8_t high = ADCH; // unlocks both
         
          long result = (high<<8) | low;
         
          result = 1125300L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
          //Serial.print(result); //debugging code
          voltage == result;
          return result; // Vcc in millivolts
          //Serial.print(result);
        }
  
  
  //checkBatt()
  
       void checkBatt()
       {
         readVcc();
         if (voltage < 3300)
         {
                prepLCD();
                gotoXY(18,1);
                LCDString("Low");
                gotoXY(10,4);
                LCDString("Battery");
               /* prepLCD();
                //Convert val from string to char              
                String str;
                str=String(voltage);
                str.toCharArray(voltage,5);
                delay(50);
                gotoXY(10,1);
                LCDString("Voltage");
                gotoXY(10,4);
                LCDString(voltage);
                //digitalWrite(LED, HIGH);
                 SOS();
                //return 0; */
         }
       }
