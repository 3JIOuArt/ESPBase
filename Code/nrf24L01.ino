void NRF_init(void) {
  radio.begin();
  radio.setDataRate(RF24_1MBPS);
  radio.setPALevel(RF24_PA_MAX);
  radio.setChannel(70);
  radio.openReadingPipe(1, 0xF0F0F0F0F0LL);
  radio.startListening();
  NRF_Reg();

  if (radio.read_register(0x00) | radio.read_register(0x03) | radio.read_register(0x04))
  {
    Serial.println("------------NRF OK-----------");
  }
  else
  {
    Serial.println("----------NRF ERROR-----------");
  }
}

void NRF_read(void) {
  int Loop = 0;
  String NRF_DATA;
  NRF_Reg();
  while (Loop < 5) {
    if (radio.available()) {
      radio.read(&Data, sizeof(Data));
      Loop = 6;
    }
    Loop++;
  }
  Serial.println(Data.id);
  Serial.println(Data.temp);
  Serial.println(Data.hum);
  Serial.println(Data.light);
  Serial.println(Data.vBat);
  Serial.println(Data.vPanel);
  Serial.println(Data.Counter);
  Serial.println("   ");
  
  time_t now = time(nullptr);
  NRF_DATA += ctime(&now);
  NRF_DATA += "Temp:";
  NRF_DATA += Data.temp;
  NRF_DATA += ", Humidity:";
  NRF_DATA += Data.hum;
  NRF_DATA +=", Ligt:";
  NRF_DATA +=Data.light;
  NRF_DATA +=", vBattery:";
  NRF_DATA +=Data.vBat;
  NRF_DATA +=", vPanel:";
  NRF_DATA +=Data.vPanel;
  NRF_DATA +=", Counter:";
  NRF_DATA +=Data.Counter;
  
  SaveData(NRF_DATA);  
}

void NRF_Reg(void) {
  Serial.print("NRF_CONFIG = ");   Serial.println(radio.read_register(0x00));
  Serial.print("EN_AA = ");        Serial.println(radio.read_register(0x01));
  Serial.print("EN_RXADDR = ");    Serial.println(radio.read_register(0x02));
  Serial.print("SETUP_AW = ");     Serial.println(radio.read_register(0x03));
  Serial.print("SETUP_RETR = ");   Serial.println(radio.read_register(0x04));
  Serial.print("RF_CH = ");        Serial.println(radio.read_register(0x05));
  Serial.print("RF_SETUP = ");     Serial.println(radio.read_register(0x06));
  Serial.print("NRF_STATUS = ");   Serial.println(radio.read_register(0x07));
}

bool SaveData(String data) {
  File NRFData = SPIFFS.open("/NRFData.txt", "a");
  if (!NRFData) {
    Serial.println("Failed to open NRFData.txt for writing");
    return false;
  }
  NRFData.println(data);
  Serial.println("---------NRFData SAVED----------");
  return true;
  NRFData.close();
  }
