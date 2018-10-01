void HTTP_init(void) {
  
  HTTP.on("/configs.json",     handle_ConfigJSON);
  HTTP.on("/nrf.json",         handle_NRFJSON);
  HTTP.on("/ScanNetwork.json", handle_ScanNetwork);
  HTTP.on("/HTTPGet.json",     handle_HTTPGet);
  HTTP.on("/restart",          handle_Restart);   
  HTTP.on("/ssid",             handle_Set_Ssid);
  HTTP.on("/ssidAP",           handle_Set_Ssid);
  HTTP.on("/HTTPRequest",      handle_HTTPRequest);
  HTTP.on("/HTTPGet",          handle_HTTPGet);
   
  HTTP.on("/update", HTTP_POST, [](){ 
  HTTP.sendHeader("Connection", "close");
  HTTP.sendHeader("Access-Control-Allow-Origin", "*");
//  HTTP.send(200, "text/plain", (Update.hasError())?"FAIL":"OK");
      ESP.restart();
    },[](){
       HTTPUpload& upload = HTTP.upload();
      if(upload.status == UPLOAD_FILE_START){
        Serial.setDebugOutput(true);
        WiFiUDP::stopAll();
       // Serial.printf("Update: %s\n", upload.filename.c_str());
        uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
        if(!Update.begin(maxSketchSpace)){//start with max available size
          Update.printError(Serial);
        }
      } else if(upload.status == UPLOAD_FILE_WRITE){
        if(Update.write(upload.buf, upload.currentSize) != upload.currentSize){
          Update.printError(Serial);
        }
      } else if(upload.status == UPLOAD_FILE_END){
        if(Update.end(true)){ //true to set the size to the current progress
         // Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
         Serial.println("Update Success");
        } else {
          Update.printError(Serial);
        }
        Serial.setDebugOutput(false);
      }
      yield();
    });
  
  HTTP.begin();
  Serial.println("-----------HTTP OK-----------");
}

void handle_ConfigJSON() {
  String json = "{";  // Формировать строку для отправки в браузер json формат
  //{"SSDP":"SSDP-test","ssid":"home","password":"i12345678","ssidAP":"WiFi","passwordAP":"","HTTPRequest":"192.168.0.104/sensors","ip":"192.168.0.101"}
  // Имя SSDP
  json += "\"SSDP\":\"";
  json += _SSDP_Name;
  // Имя сети
  json += "\",\"ssid\":\"";
  json += _ssid;
  // Пароль сети
  json += "\",\"password\":\"";
  json += _password;
  // Имя точки доступа
  json += "\",\"ssidAP\":\"";
  json += _ssidAP;
  // Пароль точки доступа
  json += "\",\"passwordAP\":\"";
  json += _passwordAP;
  // IP устройства
  json += "\",\"ip\":\"";
  json += WiFi.localIP().toString();
  json += "\"}";
  HTTP.send(200, "text/json", json);
}

void handle_NRFJSON() {
   NRF_read(); 
  String json = "{";  // Формировать строку для отправки в браузер json формат
  json += "\"ID\":\"";
  json += Data.id;
  // NRF temp
  json += "\",\"temp\":\"";
  json += Data.temp;
  // NRF hum
  json += "\",\"hum\":\"";
  json +=Data.hum;
  // NRF light
  json += "\",\"light\":\"";
  json +=Data.light;
  // NRF vBat
  json += "\",\"vBat\":\"";
  json += Data.vBat;
  // NRF vPanel
  json += "\",\"vPanel\":\"";
  json += Data.vPanel;
  // Counter
  json += "\",\"Counter\":\"";
  json += Data.Counter;
  json += "\"}";
  HTTP.send(200, "text/json", json);
}

void handle_ScanNetwork() {
  int numberOfNetworks = WiFi.scanNetworks();
  String json = "{";  
  
  json += "\"NumberOfNetworks\":\"";
  json += numberOfNetworks;

  for(int i = 0; i<numberOfNetworks; i++){
    
  json += "\"Id\":\"";
  json += i;
  
  json += "\"Network name\":\"";
  json += WiFi.SSID(i);

  json += "\"Signal strength\":\"";
  json += WiFi.RSSI(i);
}
json += "\"}";
HTTP.send(200, "text/json", json);
}
void handle_Set_Ssid() {
  _ssid = HTTP.arg("ssid");            // Получаем значение ssid из запроса сохраняем в глобальной переменной
  _password = HTTP.arg("password");    // Получаем значение password из запроса сохраняем в глобальной переменной
  saveConfig();                        // Функция сохранения данных во Flash пока пустая
  HTTP.send(200, "text/plain", "OK");   // отправляем ответ о выполнении
 Serial.println("-----------SSIDANDPASS CNAHGED-----------");
}

void handle_Set_SsidAP() {
  _ssidAP = HTTP.arg("ssidAP");            // Получаем значение ssidAP из запроса сохраняем в глобальной переменной
  _passwordAP = HTTP.arg("passwordAP");    // Получаем значение passwordAP из запроса сохраняем в глобальной переменной
  saveConfig();                        // Функция сохранения данных во Flash пока пустая
  HTTP.send(200, "text/plain", "OK");   // отправляем ответ о выполнении
 Serial.println("-----------SSIDAPANDPASS CNAHGED---------");
}


void handle_Restart() {
  Serial.println("-----------RESTART-----------");
  String restart = HTTP.arg("device");
  if (restart == "ok") {
    HTTP.send(200, "text / plain", "Reset OK");
    ESP.restart();                               
  }
  else {                                       
    HTTP.send(200, "text / plain", "No Reset"); 
  }
}

void handle_HTTPRequest() {
  HTTPRequest = HTTP.arg("HTTPRequest");
  saveConfig();                        // Функция сохранения данных во Flash пока пустая
  HTTP.send(200, "text/plain", "OK");   // отправляем ответ о выполнении
 Serial.println("-----------HTTPRequest CNAHGED-----------");
}

void handle_HTTPGet() {
 HTTPGet();
      String json = "{"; 
      json += "\"HTTPRequest\":\"";
      json += HTTPRequest;
      json += "\",\"RAW string\":\"";
      json += HTTPGET;
      json += "\"}";
      HTTP.send(200, "text/json", json);
}


