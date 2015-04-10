#if TSL2591attached
void configureTSL2591(void)
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
}

void readLight()
{
  lum = tsl.getFullLuminosity();
  ir = lum >> 16;
  full = lum & 0xFFFF;
  lux = tsl.calculateLux(full, ir);
  
  debug.print("[ "); 
  debug.print(millis()); 
  debug.print(" ms ] ");
  debug.print("IR: "); 
  debug.print(ir);  
  debug.print("  ");
  debug.print("Full: "); 
  debug.print(full); 
  debug.print("  ");
  debug.print("Visible: "); 
  debug.print(full - ir); 
  debug.print("  ");
  debug.print("Lux: "); 
  debug.println(tsl.calculateLux(full, ir));
}
#endif


