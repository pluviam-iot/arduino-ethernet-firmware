/*
   Pluviam Firmware v 1.1

   Weather Station using Arduino Ethernet with HIH6130, BMP085 and Argent 80422 sensors.

   HIH6130 code originaly created by Peter H Anderson
   BMP085 code originaly created by Jim Lindblom

   created by Saulo Matte Madalozzo (saulo.zz@gmail.com) Oct, 2011
   modified by Saulo Matte Madalozzo (saulo.zz@gmail.com) Oct, 2013
   modified by Saulo Matte Madalozzo (saulo.zz@gmail.com) May, 2014
   modified by Saulo Matte Madalozzo (saulo.zz@gmail.com) Jun, 2016

*/

/*
   ----------------------------------------------------------------------------
   "THE BEER-WARE LICENSE" (Revision 42):
   <saulo.zz@gmail.com> wrote this file. As long as you retain this notice you
   can do whatever you want with this stuff. If we meet some day, and you think
   this stuff is worth it, you can buy me a beer (or a wine) in return.
   Saulo Matte Madalozzo
   ----------------------------------------------------------------------------
*/


#define DEBUG_SERIAL true


//libs
#include <Wire.h>

//led pins
#define LED_PWR 4
#define LED_ETH 5
#define LED_CPU 6
#define LED_ERR1 7
#define LED_ERR2 7


void setup() {

  pinMode(LED_PWR, OUTPUT);
  pinMode(LED_ETH, OUTPUT);
  pinMode(LED_CPU, OUTPUT);
  pinMode(LED_ERR1, OUTPUT);
  pinMode(LED_ERR2, OUTPUT);

  digitalWrite(LED_PWR, HIGH);
  digitalWrite(LED_ETH, HIGH);
  digitalWrite(LED_CPU, HIGH);
  digitalWrite(LED_ERR1, HIGH);
  digitalWrite(LED_ERR2, HIGH);
  delay(400);
  digitalWrite(LED_ERR2, LOW);
  delay(200);
  digitalWrite(LED_ERR1, LOW);
  delay(200);
  digitalWrite(LED_CPU, LOW);
  delay(200);
  digitalWrite(LED_ETH, LOW);
  delay(200);
  digitalWrite(LED_PWR, LOW);
  delay(500);

#if DEBUG_SERIAL == true
  Serial.begin(57600);
  Serial.println(F("Pluviam firmware 1.1"));
  Serial.print(F("Starting Wire..."));
#endif
 
  digitalWrite(LED_ERR1, HIGH);
  Wire.begin();
  delay(50);
  digitalWrite(LED_ERR1, LOW);
  
#if DEBUG_SERIAL == true
  Serial.println("[OK]");
  Serial.print(F("Calibrating BMP085 and 180..."));
#endif

  digitalWrite(LED_ERR1, HIGH);
  bmp085Calibration(); //uri1
  digitalWrite(LED_ERR1, LOW);
  
  boolean initialized = false;
  unsigned long initialDelay = getInitDelay();
  unsigned long initTime = millis();
  boolean powerOn = false;

  //delay to initialize all sensors and ethernet
  while (!initialized) { 
    initTime = millis();
    if (initTime >= initialDelay) {
      initialized = true;
    }
    if ((initTime % 300) == 0)
    {
      Serial.print(initTime);
      Serial.print(" - ");
      Serial.println(initialDelay);
      if (powerOn)
        digitalWrite(LED_PWR, LOW);
      else
        digitalWrite(LED_PWR, HIGH);
      powerOn = !powerOn;
      delay(1);
    }
  }
  // digitalWrite(LED_PWR, LOW);

  #if DEBUG_SERIAL == true
  Serial.println("[OK]");
  Serial.print(F("Initializing ethernet..."));
#endif
  digitalWrite(LED_ETH, HIGH);

  startConnection();

  digitalWrite(LED_ETH, LOW);
  
#if DEBUG_SERIAL == true
  Serial.println("[OK]");
  Serial.print(F("   IP Address: "));
  printIpInfo();
  Serial.println();
  Serial.print(F("Waiting sensors... "));
  printRam();
#endif
  
#if DEBUG_SERIAL == true
  Serial.println("[OK]");
  Serial.print(F("Attaching interrupts "));
#endif

  //interrupt functions
  attachInterrupt(0, interruptWind, FALLING); //pin 2
  attachInterrupt(1, interruptRain, FALLING); //pin 3

#if DEBUG_SERIAL == true
  Serial.println("[OK]");
  Serial.println("-------");
  Serial.print("time: ");
  Serial.println(millis());
  printRam();
#endif
  //now its on
  digitalWrite(LED_PWR, HIGH);
}


float temperatureSum = 0;
long pressureSum = 0;
float humiditySum = 0;


void loop() {
  //time var will reset at 4294967295;
  unsigned long time = 0; 
  time = millis();

  //compute sensors data
  if (blockInfo(time)) { 
    digitalWrite(LED_CPU, HIGH);
#if DEBUG_SERIAL == true
    printRam();
    Serial.print("time: ");
    Serial.println(time);
#endif

    getHIH6130Data();
    temperatureSum += getTemperature();
    humiditySum += getHumidity();
    pressureSum += getPressure();
    calcWindMax();
    
#if DEBUG_SERIAL == true
    printRam();
#endif

    digitalWrite(LED_CPU, LOW);
    if (sendInfo()) { //send data
      digitalWrite(LED_ETH, HIGH);
      digitalWrite(LED_CPU, HIGH);
      
#if DEBUG_SERIAL == true
      Serial.println(F("SEND"));
#endif

      pluviamPost();

      temperatureSum = 0;
      humiditySum = 0;
      pressureSum = 0;
      digitalWrite(LED_ETH, LOW);
      digitalWrite(LED_CPU, LOW);
#if DEBUG_SERIAL == true
      printRam();
#endif
    }
  }
}

#if DEBUG_SERIAL == true
int freeRam ()
{
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}
void printRam() {
  Serial.print(F("RAM "));
  Serial.println(freeRam ());
}
#endif
