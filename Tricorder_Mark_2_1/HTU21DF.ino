

void startHTU()
{
  Serial.println("HTU21D-F test");
  if (!htu.begin())
  {
    Serial.println("Couldn't find sensor!");
    while (1);
  }
}

void readHTU()
{
  airT = htu.readTemperature();
  airH = htu.readHumidity();
  #if DEBUG
  Serial.print("Temp: "); Serial.print(airT);
  Serial.print("\t\tHum: "); Serial.println(airH);
  delay(500);
  #endif
  
  delay(50);
}
