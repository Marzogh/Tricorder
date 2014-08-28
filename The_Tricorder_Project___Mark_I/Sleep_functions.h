  #include <avr/interrupt.h>
  #include <avr/power.h>
  #include <avr/sleep.h>

  //#include <avr/io.h> //Optional include if pin control is necessary

    /* io.h lets us do a bit of housekeeping.  We don’t want rogue connections in our circuit to draw extra power from the AVR.  
    Often, any devices that are connected to output pins will draw small amounts of power even when not in use. 
    Any input pins that are floating (not using a pullup or pulldown resistor) will also consume power. 
    Below are some sample commands to manipulate ports and pins  on the Arduino.  
    For this example, we’ll set all pins besides TX (pin 0) to input mode and enable pullup resistors, 
    since there’s nothing connected right now.
    
    void setup()
    {
        DDRD &= B00000011;       // set Arduino pins 2 to 7 as inputs, leaves 0 & 1 (RX & TX) as is
        DDRB = B00000000;        // set pins 8 to 13 as inputs
        PORTD |= B11111100;      // enable pullups on pins 2 to 7, leave pins 0 and 1 alone
        PORTB |= B11111111;      // enable pullups on pins 8 to 13
        pinMode(13,OUTPUT);      // set pin 13 as an output so we can use LED to monitor
    } */
    
    
   const int pin = 2;
    
    //pininterrupt()
    void pinInterrupt(void)
      {
        // This will bring us back from sleep.
        
        // We detach the interrupt to stop it from continuously firing while the interrupt pin is low.
      }
      
      //LEDFadeOn()
      void LEDFadeOn()
      {
        for (int i=0; i<256; i++)
            {
              analogWrite(LED, i);
              delay(10);  // turn LED on
            }
      } 
 
      
      //LEDFadeOff()
      void LEDFadeOff()
      {
        for (int i=255; i>=0; i--)
            {
              analogWrite(LED, i);
              delay(10);  // turn LED off
            }
      }
      
      
    
    //sleepNow()
    void sleepNow(void)
    {
      //LEDFadeOn();
      //delay(50);
      LEDFadeOff();
      //delay(50);
      
      // Choose preferred sleep mode:
      set_sleep_mode(SLEEP_MODE_PWR_DOWN);
      
      // Set sleep enable (SE) bit:
      sleep_enable();
      
      // Set pin 2 as interrupt and attach handler:
      attachInterrupt(0, pinInterrupt, LOW);
      delay(150);
      
      // Put the device to sleep:
            
      sleep_mode();
      
      // Upon waking up, sketch continues from this point.
      sleep_disable();
      
      detachInterrupt(0);
      
      LEDFadeOn();
      delay(100);
      LEDFadeOff();
    }
