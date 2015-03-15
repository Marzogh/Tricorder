uint16_t readUV()                  //Function to measure UV - returns the value as the UV index
{
  float UV = analogRead(A1);
  UV = UV*(5.0 / 1023.0);
  uint8_t result = UV/0.1;         //Dividing the output viltage by 0.1 gives the UV index
  return result;
}

