void SpiWriteRegister(uint8_t reg, uint8_t cmd)
{
  SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
 digitalWrite(nSel, 0); //Select the radio by pulling the nSEL pin to low (nSel=10), PB2
  SPI.transfer(0x80 | reg)  ; // 0x80 is first nRW bit high (1), IMPORTANT!
  SPI.transfer(cmd); ;// command is full byte
  digitalWrite(nSel, 1); //Deselect the radio by pulling high the nSEL pin
}

uint8_t SpiReadRegister(uint8_t reg)
{
  digitalWrite(nSel, 0); //Select the radio by pulling the nSEL pin to low
  SPI.transfer(reg);
  c = SPI.transfer(0)  ; // "value" not really needed for purpose of reset flags, but...was Length
  digitalWrite(nSel, 1); //Deselect the radio by pulling high the nSEL pin
  return c;
}
void si4432_init(void) {
  SPI.begin();
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(SPI_CLOCK_DIV16);  //was 16
  SPI.setBitOrder(MSBFIRST);
  Serial.println("Start");
  delay(150);

  ItStatus1 = SpiReadRegister(0x03);  //read the Interrupt Status1 register
  ItStatus2 = SpiReadRegister(0x04);  //read the Interrupt Status2 register
  delay(150);

  SpiWriteRegister(0x07, 0x80);
  delay(150);

  while (digitalRead(NIRQ) == 1) {
    ItStatus1 = SpiReadRegister(0x03);    //read the Interrupt Status1 register
    ItStatus2 = SpiReadRegister(0x04);    //read the Interrupt Status2 register
    delay(10);
  }
  delay(150);

  Serial.println(SpiReadRegister(0x04));

SpiWriteRegister(0x75, 0x40 | 0x13); //write 0x75 to the Frequency Band Select register        0x75=117 B01110101, 0x65=101 B01100101
  SpiWriteRegister(0x76, 0x4e);  //write 0xBB to the Nominal Carrier Frequency1 register   0xBB=187, B10111011
  SpiWriteRegister(0x77, 0x00);  //write 0x80 to the Nominal Carrier Frequency0 register   0x80=128, B10000000 0xBB80 gives: 48000
  SpiWriteRegister(0x6E, 0x4E);  //write 0x4E to the TXDataRate 1 register
  SpiWriteRegister(0x6F, 0xA5);  //write 0xA5 to the TXDataRate 0 register
  SpiWriteRegister(0x70, 0x2C);  //write 0x2C to the Modulation Mode Control 1 register
  SpiWriteRegister(0x72, 0x10);  //write 0x48 to the Frequency Deviation register
  SpiWriteRegister(0x1C, 0x12);  //write 0x1E to the IF Filter Bandwidth register
  SpiWriteRegister(0x20, 0xD0);  //write 0xD0 to the Clock Recovery Oversampling Ratio register
  SpiWriteRegister(0x21, 0x00);  //write 0x00 to the Clock Recovery Offset 2 register
  SpiWriteRegister(0x22, 0x9D);  //write 0x9D to the Clock Recovery Offset 1 register
  SpiWriteRegister(0x23, 0x49);  //write 0x49 to the Clock Recovery Offset 0 register
  SpiWriteRegister(0x24, 0x00);  //write 0x00 to the Clock Recovery Timing Loop Gain 1 register
  SpiWriteRegister(0x25, 0x24);  //write 0x24 to the Clock Recovery Timing Loop Gain 0 register
  SpiWriteRegister(0x1D, 0x40);  //write 0x40 to the AFC Loop Gearshift Override register
  SpiWriteRegister(0x1E, 0x0A);  //write 0x0A to the AFC Timing Control register
  SpiWriteRegister(0x2A, 0x20);   //write 0x20 to the AFC Limiter register
  SpiWriteRegister(0x34, 0x0A);   //write 0x0A to the Preamble Length register
  SpiWriteRegister(0x35, 0x2A);   //write 0x2A to the Preamble Detection Control  register
  SpiWriteRegister(0x33, 0x02);  //write 0x02 to the Header Control2 register
  SpiWriteRegister(0x36, 0x2D);  //write 0x2D to the Sync Word 3 register
  SpiWriteRegister(0x37, 0xD4);  //write 0xD4 to the Sync Word 2 register
  SpiWriteRegister(0x30, 0x8D);   //write 0x8D to the Data Access Control register
  SpiWriteRegister(0x32, 0x00);   //write 0x00 to the Header Control1 register
  SpiWriteRegister(0x71, 0x63);   //write 0x63 to the Modulation Mode Control 2 register
  SpiWriteRegister(0x0b, 0x12); //write 0x12 to the GPIO1 Configuration(set the TX state)
  SpiWriteRegister(0x0c, 0x15); //write 0x15 to the GPIO2 Configuration(set the RX state)
  SpiWriteRegister(0x09, 0xAC); //write 0xD7 to the Crystal Oscillator Load Capacitance register
  SpiWriteRegister(0x69, 0x60); //write 0x60 to the AGC Override1 register
  SpiWriteRegister(0x07, 0x05); //write 0x05 to the Operating Function Control 1 register
  SpiWriteRegister(0x05, 0x03);  //write 0x03 to the Interrupt Enable 1 register
  SpiWriteRegister(0x06, 0x00);  //write 0x00 to the Interrupt Enable 2 register
  
  delay(150);
  ItStatus1 = SpiReadRegister(0x03);  //read the Interrupt Status1 register
  ItStatus2 = SpiReadRegister(0x04);  //read the Interrupt Status2 register
  delay(150); 
  SpiWriteRegister(0x6D, 0x07); // 0x00 is minimum power, 0x07 is max power! was: (0x6D,0x08|0x00)

  Serial.println(ItStatus1);
  Serial.println(ItStatus2);
}

void chip_ID(void) {
  uint8_t version;
  SpiWriteRegister(0x07, 0x1);
  delay(150);
  version = SpiReadRegister(0x01);
  SpiWriteRegister(0x07, 0x05);
  Serial.print("Chip version - ");
  switch (version) {
  case 6:
    Serial.println("Si4430/31/32 Rev B1");
    break;
  case 7:
    Serial.println("Si100x Rev E, Si101x Rev B: Si102x/3x Rev B");
    break;
  default:
    Serial.println("Unable read chip version");
    break;
}
}

void si4432_send(void) {
  SpiWriteRegister(0x07, 0x01); //write 0x01 to the Operating Function Control 1 register
  delay(150);
  
  SpiWriteRegister(0x3E, sizeof(payload)); //write 8 to the Transmit Packet Length register
  delay(150);
  
  for (int i; i <= sizeof(payload) - 1; i++) {
    SpiWriteRegister(0x7F, payload[i]); //write to the FIFO Access register
     Serial.print(payload[i]); 
  }
  Serial.println(" "); 
  delay(150);

  SpiWriteRegister(0x05, 0x04); //write 0x04 to the Interrupt Enable 1 register
  SpiWriteRegister(0x06, 0x00); //write 0x03 to the Interrupt Enable 2 registerItStatus1 = SpiReadRegister(0x03); //read the Interrupt Status1 register
  delay(150);
  
  ItStatus2 = SpiReadRegister(0x04); //read the Interrupt Status2 register
  SpiWriteRegister(0x07, 0x09);  //write 0x09 to the Operating Function Control 1 register
  delay(150);
  
  while (digitalRead(NIRQ) == 1) {
    ItStatus1 = SpiReadRegister(0x03); //read the Interrupt Status1 register
    ItStatus2 = SpiReadRegister(0x04); //read the Interrupt Status2 register
     delay(10);
  }
  Serial.println(" NIRQ - OK"); 
  delay(10);  //was delay(10) - DO NOT increase on single end!
  
  SpiWriteRegister(0x05, 0x03); //write 0x03 to the Interrupt Enable 1 register
  SpiWriteRegister(0x06, 0x00); //write 0x00 to the Interrupt Enable 2 register

  ItStatus1 = SpiReadRegister(0x03); //read the Interrupt Status1 register
  ItStatus2 = SpiReadRegister(0x04); //read the Interrupt Status2 register

  SpiWriteRegister(0x07, 0x05);  //write 0x05 to the Operating Function Control 1 register
  delay(150);
  Serial.println(" Sended"); 
}
