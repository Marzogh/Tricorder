/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~ TSL2591 code ~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
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

  /* Display the gain and integration time for reference sake */  
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
  // Get a new sensor event
  sensors_event_t event;
  tsl.getEvent(&event);
 
  // Display the results (light is measured in lux)
  Serial.print("[ "); Serial.print(event.timestamp); Serial.print(" ms ] ");
  if ((event.light == 0) |
      (event.light > 4294966000.0) | 
      (event.light <-4294966000.0))
  {
    // If event.light = 0 lux the sensor is probably saturated
    // and no reliable data could be generated!
    // if event.light is +/- 4294967040 there was a float over/underflow
    Serial.println("Invalid data (adjust gain or timing)");
    blinkError(3);
  }
  else
  {
    lux = event.light;
    Serial.print(event.light); Serial.println(" lux");
  }
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ SHT1x code ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void readSoil()
{
  soilT = sht1x.readTemperatureC();
  blinkSuccess(1);
  soilH = sht1x.readHumidity();
  blinkSuccess(1);

  Serial.print(F("Temperature: "));
  Serial.print(soilT, DEC);
  Serial.print(F("C"));
  Serial.print(F("Humidity: "));
  Serial.print(soilH);
  Serial.println(F("%"));
  delay(2000);
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~ HTU21DF code ~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void readAir()
{
  airH = air.readHumidity();
  blinkSuccess(1);
  airT = air.readTemperature();
  blinkSuccess(1);

  Serial.print(F("Time:"));
  Serial.print(millis());
  Serial.print(F(" Temperature:"));
  Serial.print(airT, 1);
  Serial.print(F("C"));
  Serial.print(F(" Humidity:"));
  Serial.print(airH, 1);
  Serial.print(F("%"));

  Serial.println();
  delay(1000);
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~~~~~~~~~~~Encoder functions~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
