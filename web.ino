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
// modified by Saulo Matte Madalozzo (saulo.zz@gmail.com) Apr, 2014
// modified by Saulo Matte Madalozzo (saulo.zz@gmail.com) May, 2014


#include <SPI.h>
#include <Ethernet.h>
EthernetClient client;

const char serverUrl[] = "backyard.pluvi.am";
const String stationId = "57675da7894057a155fa0e97";
const String token = "53tkinxigizstd2r";
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFF, 0xEF };

String postData = "";

void startConnection() {
  if (Ethernet.begin(mac) == 0) { //dhcp
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    digitalWrite(LED_ERR1, HIGH);
    digitalWrite(LED_ETH, HIGH);
    while (true);
  }
}


void setPostData(String dataLayer, String value, boolean init, boolean finish) {
  if (init && !finish) {
    postData = "{\"" + dataLayer + "\":" + value + ",";
  }
  else if (!init && !finish) {
    postData = postData + "\"" + dataLayer + "\":" + value + ",";
  }
  else if (!init && finish) {
    postData = postData + "\"" + dataLayer + "\":" + value + "}";
  }
  else {
    postData = "{\"" + dataLayer + "\":" + value + "}";
  }
}

void pluviamMakePost() {
  client.println("POST /stations/" + stationId + " HTTP/1.0");
  client.println("Host: " + String(serverUrl));
  client.println("User-Agent: Arduino/1.0");
  client.println("Connection: close");
  client.println("X-Pluviam-Token: " + token);
  client.println("Content-Type: application/json");
  client.print("Content-Length: ");
  client.println(postData.length());
  client.println();
  client.println(postData);
}

void pluviamPost()
{
  Ethernet.maintain();

  setPostData(F("temperature"), String(temperatureSum / BLOCK_COUNT), true, false);
#if DEBUG_SERIAL == true
  Serial.print("temp ");
  printRam();
#endif
  setPostData(F("humidity"), String(humiditySum / BLOCK_COUNT), false, false);
#if DEBUG_SERIAL == true
  Serial.print("humi ");
  printRam();
#endif
  setPostData(F("pressure"), String(pressureSum / BLOCK_COUNT), false, false);
#if DEBUG_SERIAL == true
  Serial.print("press ");
  printRam();
#endif
  setPostData(F("precipitation"), String(getRain()), false, false);
#if DEBUG_SERIAL == true
  Serial.print("precipt ");
  printRam();
#endif
  setPostData(F("windSpeed"), String(getWind()), false, false);
#if DEBUG_SERIAL == true
  Serial.print("windSp ");
  printRam();
#endif
  setPostData(F("windGust"), String(getWindMax()), false, false);
#if DEBUG_SERIAL == true
  Serial.print("windGus ");
  printRam();
#endif
  setPostData(F("windDirection"), String(convertedWind()), false, true);
#if DEBUG_SERIAL == true
  Serial.print("windDir ");
  printRam();
#endif



#if DEBUG_SERIAL == true
  Serial.println(postData);
#endif

  client.stop();
  if (client.connect(serverUrl, 80)) {
#if DEBUG_SERIAL == true
    Serial.println(F("connected"));
#endif
    pluviamMakePost();
#if DEBUG_SERIAL == true    
  } else {
    Serial.println(F("connection failed"));
#endif
  }
  delay(500);
#if DEBUG_SERIAL == true  
  while (client.available()) {
    char c = client.read();
    Serial.print(c);
  }
  Serial.println(F("done!"));
#endif  
  if (!client.connected()) {
    client.stop();  // DISCONNECT FROM THE SERVER
#if DEBUG_SERIAL == true  
    Serial.println(F("stoped"));
#endif
  }

}


#if DEBUG_SERIAL == true
void printIpInfo()
{
  Serial.print(Ethernet.localIP());
}
#endif


