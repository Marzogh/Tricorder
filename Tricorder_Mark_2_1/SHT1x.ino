
void readSHT()
{
  // Read values from the sensor
  soilT = sht1x.readTemperatureC();
  //temp_f = sht1x.readTemperatureF();
  soilH = sht1x.readHumidity();

  // Print the values to the serial port
  #if DEBUG
  Serial.print("Temperature: ");
  Serial.print(soilT, DEC);
  Serial.print("C / ");
  //Serial.print(temp_f, DEC);
  //Serial.print("F."
  Serial.print("Humidity: ");
  Serial.print(soilH);
  Serial.println("%");
  #endif
  
  delay(200);
}
