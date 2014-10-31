
float airT;
float airH;

Adafruit_HTU21DF htu = Adafruit_HTU21DF();

void prepHTU()
{
 if(!htu.begin())
 {
   Serial.println("Cannot initiate HTU21DF");
 }
  delay (10);
}

void readAir()
{
  airH = htu.readHumidity();
  airT = htu.readTemperature();
  Serial.print("[ "); Serial.print(millis()); Serial.print(" ms ] ");
  Serial.print("Air Humidity : ");
  Serial.print(airH);
  Serial.print("\t\t");
  Serial.print("Air Temp : ");
  Serial.println(airT);
  delay(20);
}
