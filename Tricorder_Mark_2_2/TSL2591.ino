void readLight()
{
  lum = tsl.getFullLuminosity();
  ir = lum >> 16;
  full = lum & 0xFFFF;
  Serial.print("[ "); 
  Serial.print(millis()); 
  Serial.print(" ms ] ");
  Serial.print("IR: "); 
  Serial.print(ir);  
  Serial.print("  ");
  Serial.print("Full: "); 
  Serial.print(full); 
  Serial.print("  ");
  Serial.print("Visible: "); 
  Serial.print(full - ir); 
  Serial.print("  ");
  Serial.print("Lux: "); 
  Serial.println(tsl.calculateLux(full, ir));
}

