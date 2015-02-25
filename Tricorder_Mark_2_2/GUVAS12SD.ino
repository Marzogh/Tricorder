uint16_t readUV()
{
  float UV = analogRead(A1);
  UV = UV*4.3;
  uint8_t result = UV*10;
  return result;
}

