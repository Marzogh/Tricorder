void readSoil()
{
  // Read values from the sensor
  soilT = sht1x.readTemperatureC();
  debug.print("Soil Temp: ");
  debug.print(soilT, 2);
  //temp_f = sht1x.readTemperatureF();
  soilH = sht1x.readHumidity();
  debug.print(" Soil Humidity: ");
  debug.println(soilH, 2);
}
