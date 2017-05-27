/* This file is part of the Pluviam Firmware */

/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <saulo.zz@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer (or a wine) in return.
 * Saulo Matte Madalozzo
 * ----------------------------------------------------------------------------
 */

// created by Saulo Matte Madalozzo (saulo.zz@gmail.com) Oct, 2011
// modified by Saulo Matte Madalozzo (saulo.zz@gmail.com) Oct, 2013
 
volatile unsigned int rain = 0;
volatile unsigned long lastInterruptTimeRain = 0;
volatile unsigned long interruptTimeRain = 0;

 void interruptRain()
{
    interruptTimeRain = millis();
  // If interrupts come faster than 100ms, assume it's a bounce and ignore
  if (interruptTimeRain - lastInterruptTimeRain > 100)
  {
    rain++;  
#if DEBUG_SERIAL == true
    Serial.println(F("rain" ));
#endif
  }
  lastInterruptTimeRain = interruptTimeRain;
}

unsigned int getRain(){
  unsigned int result = rain;
  rain = 0;
  return result;
}
