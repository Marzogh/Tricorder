  
  //Define Sensor Power pin
  //const int SHT_PWR=3;      //Connect Vin Digital Pin 2 (3.3-5V DC)
  
  float soilT;
  //float temp_f;
  float soilH;
  
  // connect data wire to digital 3
  // connect clock wire to digital 4
  // connect GROUND to common ground
  
  // Specify data and clock connections and instantiate SHT1x object
  #define dataPin  12
  #define clockPin 13
  SHT1x sht1x(dataPin, clockPin);
  
  //Sensor functions
  
  void readSoil()
  {
    // Read values from the sensor
    soilT = sht1x.readTemperatureC();
    //temp_f = sht1x.readTemperatureF();
    soilH = sht1x.readHumidity();
    delay(2000);
  
  // Debugging code foir SHT1x begins
    Serial.print("[ "); Serial.print(millis()); Serial.print(" ms ] ");
    
    // Print the values to the serial port
    Serial.print("Temperature: ");
    Serial.print(soilT, DEC);
    Serial.print("C / ");
    //Serial.print(temp_f, DEC);
    //Serial.print("F. Humidity: ");
    Serial.print(soilH);
    Serial.println("%");
  // Debugging code foir SHT1x ends
  
    delay(5);
  }
