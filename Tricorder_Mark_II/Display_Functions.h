//#include <genieArduino.h>
//#include "SD_Logger_Functions.h"



  
  /* //Code for event handler (Longhand version)
void myGenieEventHandler()
{
  genieFrame Event;
  genie.DequeueEvent(&Event);

  int slider_val = 0;

  //If the cmd received is from a Reported Event (Events triggered from the Events tab of Workshop4 objects)
  if (Event.reportObject.cmd == GENIE_REPORT_EVENT)
  {
    if (Event.reportObject.object == GENIE_OBJ_SLIDER)                // If the Reported Message was from a Slider
    {
      if (Event.reportObject.index == 0)                              // If Slider0
      {
        slider_val = genie.GetEventData(&Event);                      // Receive the event data from the Slider0
        genie.WriteObject(GENIE_OBJ_LED_DIGITS, 0x00, slider_val);    // Write Slider0 value to to LED Digits 0
      }
    }
  }

  //If the cmd received is from a Reported Object, which occurs if a Read Object (genie.ReadOject) is requested in the main code, reply processed here.
  if (Event.reportObject.cmd == GENIE_REPORT_OBJ)
  {
    if (Event.reportObject.object == GENIE_OBJ_USER_LED)              // If the Reported Message was from a User LED
    {
      if (Event.reportObject.index == 0)                              // If UserLed0
      {
        bool UserLed0_val = genie.GetEventData(&Event);               // Receive the event data from the UserLed0
        UserLed0_val = !UserLed0_val;                                 // Toggle the state of the User LED Variable
        genie.WriteObject(GENIE_OBJ_USER_LED, 0x00, UserLed0_val);    // Write UserLed0_val value back to to UserLed0
      }
    }
  }
} */


