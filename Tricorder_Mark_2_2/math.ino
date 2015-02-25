  boolean compareVariables()
  {
    if (prev_airT == airT && prev_airH == airH && prev_soilT == soilT && prev_soilH == soilH)
    return true;
    else
    {
      lcd.print(F("Reading sensors..."));
      return false;
    }
  }
  
  void resetVariables()
  {
    prev_airT, prev_airH, prev_soilT, prev_soilH = 0;
    newRead = false;
  }
  
  void shiftVariables()
  {
    prev_airT = airT;
    prev_airH = airH;
    prev_soilT = soilT;
    prev_soilH = soilH;
  }
