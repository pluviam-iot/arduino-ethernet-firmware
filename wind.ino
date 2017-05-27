/* This file is part of the Pluviam Firmware */

/*
   ----------------------------------------------------------------------------
   "THE BEER-WARE LICENSE" (Revision 42):
   <saulo.zz@gmail.com> wrote this file. As long as you retain this notice you
   can do whatever you want with this stuff. If we meet some day, and you think
   this stuff is worth it, you can buy me a beer (or a wine) in return.
   Saulo Matte Madalozzo
   ----------------------------------------------------------------------------
*/

// created by Saulo Matte Madalozzo (saulo.zz@gmail.com) Oct, 2011
// modified by Saulo Matte Madalozzo (saulo.zz@gmail.com) Oct, 2013

//outras variaveis
volatile int wind = 0;
volatile unsigned long lastInterruptTimeWind = 0;
volatile unsigned long interruptTimeWind = 0;

int windMax = 0;
int lastWind = 0;
volatile int dirVento[9];

void interruptWind()
{

  interruptTimeWind = millis();
  // If interrupts come faster than 5ms, assume it's a bounce and ignore
  if (interruptTimeWind - lastInterruptTimeWind > 5)
  {
    wind++;
#if DEBUG_SERIAL == true      
    Serial.println(F("wind"));
#endif
    dirVento[windDirection()]++;

  }
  lastInterruptTimeWind = interruptTimeWind;

}

int getWindDirection()
{
  int bigger = 8;
  for (int i = 0; i < 8; i++)
  {
    if (dirVento[i] > dirVento[bigger])
      bigger = i;
  }
  for (int i = 0; i <= 8; i++) // zera o array
  {
    dirVento[i] = 0;
  }
  return bigger;
}

int windDirection()
{
  int aux = analogRead(1);
  if (aux < 99)
    return 2; //O

  else if (100 < aux && aux < 150)
    return 4; //NO

  else if (160 < aux && aux < 219)
    return 2; //O

  else if (220 < aux && aux < 279)
    return 0; //N

  else if (280 < aux && aux < 349)
    return 4; //NO

  else if (350 < aux && aux < 450)
    return 6; //SO

  else if (535 < aux && aux < 589)
    return 5; //NE

  else if (590 < aux && aux < 650)
    return 0; //N

  else if (700 < aux && aux < 799)
    return 1; //S

  else if (800 < aux && aux < 915)
    return 7; //SE

  else if (916 < aux)
    return 3; //E

  else
    return 8; //?
}

String convertWindString[] = {"N", "S", "W", "E", "NW", "NE", "SW", "SE", "-"};

String convertedWind() {
  return "\"" + convertWindString[getWindDirection()] + "\"";
}

String windDirectionDebug()
{
  int aux = analogRead(1);
  if (aux < 99)
    return "O";//2; //O

  else if (100 < aux && aux < 150)
    return "NO"; //NO

  else if (160 < aux && aux < 219)
    return "O"; //O

  else if (220 < aux && aux < 279)
    return "N"; //N

  else if (280 < aux && aux < 349)
    return "NO"; //NO

  else if (350 < aux && aux < 450)
    return "SO"; //SO

  else if (535 < aux && aux < 589)
    return "NE"; //NE

  else if (590 < aux && aux < 650)
    return "N"; //N

  else if (700 < aux && aux < 799)
    return "S"; //S

  else if (800 < aux && aux < 915)
    return "SE"; //SE

  else if (916 < aux)
    return "E"; //E

  else
    return "?"; //?

}

unsigned int getWind() {
  unsigned int result = wind;
  wind = 0;
  return result;
}

unsigned int getWindMax() {
  unsigned int result = windMax;
  windMax = 0;
  lastWind = 0;
  return result;
}

void calcWindMax() {
  int windCalculatedMax = wind - lastWind;
  if (windCalculatedMax > windMax) {
    windMax = windCalculatedMax;
  }
  lastWind = wind;
}


