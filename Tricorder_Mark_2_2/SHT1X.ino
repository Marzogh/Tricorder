void readSoil()
{
  // Read values from the sensor
  soilT = sht1x.readTemperatureC();
  //temp_f = sht1x.readTemperatureF();
  soilH = sht1x.readHumidity();
}
