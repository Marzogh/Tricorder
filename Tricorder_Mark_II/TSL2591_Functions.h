 /* TSL25911 Digital Light Sensor */
 /* Dynamic Range: 600M:1 */
 /* Maximum Lux: 88K */ 
  
  //Define Sensor Power pin
  //const int TSL_PWR=6;      //Connect Vin Digital Pin 5 (3.3-5V DC)
  uint16_t ir, full, vis, lux;
  
  /* Create sensor object */
  // connect SCL to analog 5
  // connect SDA to analog 4
  // connect Vin to 3.3-5V DC
  // connect GROUND to common ground
  Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591); // pass in a number for the sensor identifier (for your use later)
  
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
  
    /* Display the gain and integration time for debugging */  
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
  }
  
  
  void readLight()
  { 
    uint32_t lum = tsl.getFullLuminosity();
    ir = lum >> 16;
    full = lum & 0xFFFF;
    vis = full - ir;
    lux = tsl.calculateLux(full, ir);
    
    
    Serial.print("[ "); Serial.print(millis()); Serial.print(" ms ] ");
    Serial.print("IR: "); Serial.print(ir);  Serial.print("  ");
    Serial.print("Full: "); Serial.print(full); Serial.print("  ");
    Serial.print("Visible: "); Serial.print(full - ir); Serial.print("  ");
    Serial.print("Lux: "); Serial.println(lux);
    delay (100);
  }
  
  /**************************************************************************/
  /*
      Shows how to perform a basic read on visible, full spectrum or
      infrared light (returns raw 16-bit ADC values)
  */
  /**************************************************************************/
/*  void simpleRead(void)
  {
    // Simple data read example. Just read the infrared, fullspecrtrum diode 
    // or 'visible' (difference between the two) channels.
    // This can take 100-600 milliseconds! Uncomment whichever of the following you want to read
    uint16_t x = tsl.getLuminosity(TSL2591_VISIBLE);
    //uint16_t x = tsl.getLuminosity(TSL2561_FULLSPECTRUM);
    //uint16_t x = tsl.getLuminosity(TSL2561_INFRARED);
  
    Serial.print("[ "); Serial.print(millis()); Serial.print(" ms ] ");
    Serial.print("Luminosity: ");
    Serial.println(x, DEC);
  } */
  
  /**************************************************************************/
  /*
      Show how to read IR and Full Spectrum at once and convert to lux
  */
  /**************************************************************************/
/*  void advancedRead(void)
  {
    // More advanced data read example. Read 32 bits with top 16 bits IR, bottom 16 bits full spectrum
    // That way you can do whatever math and comparisons you want!
    uint32_t lum = tsl.getFullLuminosity();
    //uint16_t ir, full;
    ir = lum >> 16;
    full = lum & 0xFFFF;
    Serial.print("[ "); Serial.print(millis()); Serial.print(" ms ] ");
    Serial.print("IR: "); Serial.print(ir);  Serial.print("  ");
    Serial.print("Full: "); Serial.print(full); Serial.print("  ");
    Serial.print("Visible: "); Serial.print(full - ir); Serial.print("  ");
    Serial.print("Lux: "); Serial.println(tsl.calculateLux(full, ir));
  } */

