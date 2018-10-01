void WIFIinit() {
  WiFi.mode(WIFI_STA);
  byte tries = 11;
  WiFi.begin(_ssid.c_str(), _password.c_str());
  while (--tries && WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
  }
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("");
    Serial.println("WiFi up AP");
    StartAPMode();
  }
  else {
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.println("-----------WIFI OK-----------");
    SignalStrength();
  }
}

bool StartAPMode()
{ 
  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(_ssidAP.c_str(), _passwordAP.c_str());
  return true;
}

void SignalStrength() {
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
