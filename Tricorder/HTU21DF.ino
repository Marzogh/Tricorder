void readAir()                                //Reads values from the HTU21DF breakout board exposed to the air
{
  airT = htu.readTemperature();
  debug.print("Air Temp: ");
  debug.print(soilT);
  airH = htu.readHumidity();
  debug.print(" Air Humidity: ");
  debug.println(airH);
}
