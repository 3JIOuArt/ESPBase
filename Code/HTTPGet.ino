void HTTPGet(void){
    Serial.println("--------HTTPGET START---------");    
    HTTPClient http;
   http.begin(HTTPRequest.c_str());
    int httpCode = http.GET();
    
    if (httpCode > 0) { //Check the returning code
      HTTPGET = http.getString();  
      Serial.println(HTTPGET);
      http.end();  
    }
      else {
            Serial.println("----Wrong request or address----");
      }            
}
