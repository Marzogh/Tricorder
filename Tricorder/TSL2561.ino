#if TSL2561attached
void configureTSL2561()
{
  // The light sensor has a default integration time of 402ms,and a default gain of low (1X).
  // If you would like to change either of these, you can do so using the setTiming() command.
  // If gain = false (0), device is set to low gain (1X)
  // If gain = high (1), device is set to high gain (16X)

  gain = 0;

  // If time = 0, integration will be 13.7ms
  // If time = 1, integration will be 101ms
  // If time = 2, integration will be 402ms
  // If time = 3, use manual start / stop to perform your own integration

  unsigned char time = 2;

  // setTiming() will set the third parameter (ms) to the
  // requested integration time in ms (this will be useful later):

  Serial.println("Set timing...");
  tsl.setTiming(gain,time,ms); 
  tsl.setPowerUp();
}

void readLight()
{
  uint16_t vis, ir;
  delay(ms);
  if (tsl.getData(vis,ir))
  {
    boolean good;  // True if neither sensor is saturated
    good = tsl.getLux(gain,ms,vis,ir,lux);
    debug.print("[ "); 
    debug.print(millis()); 
    debug.print(" ms ] ");
    debug.print("IR: "); 
    debug.print(ir);  
    debug.print("  ");
    debug.print("Full: "); 
    debug.print(vis + ir); 
    debug.print("  ");
    debug.print("Visible: "); 
    debug.print(vis); 
    debug.print("  ");
    debug.print("Lux: "); 
    debug.println(lux);
  }
}
#endif
