#if AM2315attached
void readAir()                                //Reads values from the HTU21DF breakout board exposed to the air
{
  airT = am2315.readTemperature();
  debug.print("Air Temp: ");
  debug.print(soilT);
  airH = am2315.readHumidity();
  debug.print(" Air Humidity: ");
  debug.println(airH);
}
#endif
