void SSDP_init(void) {
  // SSDP дескриптор
  HTTP.on("/description.xml", HTTP_GET, []() {
    SSDP.schema(HTTP.client());
  });
  //Если версия  2.0.0 закаментируйте следующую строчку
  SSDP.setDeviceType("upnp:rootdevice");
  SSDP.setSchemaURL("description.xml");
  SSDP.setHTTPPort(80);
  SSDP.setName(_SSDP_Name);
  SSDP.setSerialNumber("000012346");
  SSDP.setURL("/");
  SSDP.setModelName("SSDP");
  SSDP.setModelNumber("000000000001");
  SSDP.setModelURL("puchok");
  SSDP.setManufacturer("");
  SSDP.setManufacturerURL("");
  SSDP.begin();
}
