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
#include <Wire.h>

bool Relay = 1;
uint8_t cmd;
byte NIRQ = D1;
byte nSel = D8;

byte ItStatus1,ItStatus2;
byte Length,c;

uint8_t payload [6] = {0x48, 0x65, 0x6c, 0x6c, 0x6f};
uint8_t temp8;


struct tm * ptm;
String TimeString;
String DateString;
String i2cStr;

IPAddress apIP(192, 168, 4, 1);
ESP8266WebServer HTTP(80);
#define BUFF_SIZE 64

RF24 radio(D1, D2);
#define chipSele D8;

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
String HTTPRequest   = "192.168.100.100/sensors";

String HTTPGET   = "{}";
String jsonConfig = "{}";
String jsonNRF = "{}";
String BMP280 = "{}";

#define localPort 2390      // local port to listen for UDP packets
WiFiUDP Udp;

unsigned long previousMillis = 0;
const long interval = 5000;
char Filename[16];

void setup() {
  Serial.begin(115200); 
  FS_init();
  loadConfig();
  Serial.println("");
  WIFIinit();
  SSDP_init();
  HTTP_init();
  NRF_init();
  NRF_Reg();
  Time_init(); 
  //si4432_init();
  i2c_scanner();
  BMP280_init();
  if ( MDNS.begin ( "esp8266" ) ) {
  }
  Udp.begin(localPort);

}

void loop() {
   unsigned long currentMillis = millis();
  if(currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;  
    NRF_read();
    radio.stopListening();
    radio.write(&Relay,sizeof(bool));  
    radio.startListening();
    //si4432_send();
  }
   HTTP.handleClient();
}
