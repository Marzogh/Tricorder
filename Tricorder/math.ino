float Average (float current, float prev)
{
  float runningValue = current;
  runningValue += prev;
  runningValue /= 2;
  return runningValue;
}


void resetVariables()
{
  prev_airT, prev_airH, prev_soilT, prev_soilH = 0;
  //newRead = !newRead;
}


void shiftVariables()
{
  prev_airT = airT;
  prev_airH = airH;
  prev_soilT = soilT;
  prev_soilH = soilH;
}

float errorPercent(float prev, float current)
{
  float error = (prev - current);
  if (error < 0)
  {
    error = -error;
  }
  error = error/current;
  error = error*100;
  return error;
}

boolean standardError()
{
  uint16_t errorAirT = int (errorPercent(prev_airT, airT));
  uint16_t errorAirH = int (errorPercent(prev_airH,airH));
  uint16_t errorSoilT = int (errorPercent(prev_soilT, soilT));
  uint16_t errorSoilH = int (errorPercent(prev_soilH, soilH)); 

  if (errorAirT, errorAirH, errorSoilT, errorSoilH <=5)
    return true;
  else
    return false;
}

boolean compareVariablesPercent()
{
  if (standardError)
    return true;
  else
  {
    lcd.clear();
    lcd.print(F("Reading sensors..."));
    return false;
  }
}

boolean compareVariables()
{
  uint16_t errorAirT = int (abs(prev_airT-airT));
  uint16_t errorAirH = int (abs(prev_airH-airH));
  uint16_t errorSoilT = int (abs(prev_soilT-soilT));
  uint16_t errorSoilH = int (abs(prev_soilH-soilH)); 
  if (errorAirT < 1 && errorSoilT < 1 && errorAirH < 1 && errorSoilH < 1)
  {
    airT = Average(airT, prev_airT);
    airH = Average(airH, prev_airH);
    soilT = Average(soilT, prev_soilT);
    soilH = Average(soilH, prev_soilH);
  }
}

/*int averageRead(float reading)
 {
 byte numberOfReadings = 8;
 unsigned int runningValue = 0; 
 
 for(int x = 0 ; x < numberOfReadings ; x++)
 runningValue += reading
 runningValue /= numberOfReadings;
 
 return(runningValue);  
 }*/


