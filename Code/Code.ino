#include <ESP8266WiFi.h>        
#include <ESP8266WebServer.h>   
#include <ESP8266SSDP.h>       
#include <ESP8266mDNS.h>
#include <ESP8266HTTPClient.h>
#include <FS.h>             
#include <ArduinoJson.h> 
#include <SPI.h>
#include "RF24.h"
#include <WiFiUdp.h>
#include <time.h>

int timezone = -5;
int dst = 0;

IPAddress apIP(192, 168, 4, 1);
ESP8266WebServer HTTP(80);

#define BUFF_SIZE 64

RF24 radio(D1, D2);

const int    chipSelect = D8;

struct dataStruct{
   float id;
   float temp;
   float hum;
   float light;
   float vBat;
   float vPanel;
   float Counter;
}Data;
                
File fsUploadFile;

String _ssid       = "mtsgt280";
String _password   = "2048230907";
String _ssidAP     = "MTTEST";
String _passwordAP = "2048230907";
String _SSDP_Name   = "ESPOST";
String HTTPRequest   = "192.168.100.101/sensors";

String HTTPGET   = "{}";
String jsonConfig = "{}";
String jsonNRF = "{}";

unsigned int localPort = 2390;      // local port to listen for UDP packets
IPAddress timeServer(129, 6, 15, 28); // time.nist.gov NTP server
const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message
byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets
WiFiUDP Udp;

unsigned long previousMillis = 0;
const long interval = 5000;


void setup() {
  Serial.begin(115200); 
  FS_init();
  loadConfig();
  Serial.println("");
  WIFIinit();
  SSDP_init();
  HTTP_init();
  NRF_init();
  Time_init();
  if ( MDNS.begin ( "esp8266" ) ) {
  }
  Udp.begin(localPort);

}

void loop() {
   unsigned long currentMillis = millis();
  if(currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;   
    NRF_read();
  }
   HTTP.handleClient();
//  GetTime();
}
