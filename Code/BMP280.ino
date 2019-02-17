#define Bmp280Addr 0x76
int32_t t_fine;

typedef struct
{
  uint16_t dig_T1;
  int16_t  dig_T2;
  int16_t  dig_T3;

  uint16_t dig_P1;
  int16_t  dig_P2;
  int16_t  dig_P3;
  int16_t  dig_P4;
  int16_t  dig_P5;
  int16_t  dig_P6;
  int16_t  dig_P7;
  int16_t  dig_P8;
  int16_t  dig_P9;

  uint8_t  dig_H1;
  int16_t  dig_H2;
  uint8_t  dig_H3;
  int16_t  dig_H4;
  int16_t  dig_H5;
  int8_t   dig_H6;
} bmp280_calib_data;

enum
{
  BMP280_REGISTER_DIG_T1              = 0x88,
  BMP280_REGISTER_DIG_T2              = 0x8A,
  BMP280_REGISTER_DIG_T3              = 0x8C,

  BMP280_REGISTER_DIG_P1              = 0x8E,
  BMP280_REGISTER_DIG_P2              = 0x90,
  BMP280_REGISTER_DIG_P3              = 0x92,
  BMP280_REGISTER_DIG_P4              = 0x94,
  BMP280_REGISTER_DIG_P5              = 0x96,
  BMP280_REGISTER_DIG_P6              = 0x98,
  BMP280_REGISTER_DIG_P7              = 0x9A,
  BMP280_REGISTER_DIG_P8              = 0x9C,
  BMP280_REGISTER_DIG_P9              = 0x9E,

  BMP280_REGISTER_CHIPID             = 0xD0,
  BMP280_REGISTER_VERSION            = 0xD1,
  BMP280_REGISTER_SOFTRESET          = 0xE0,

  BMP280_REGISTER_CAL26              = 0xE1,  // R calibration stored in 0xE1-0xF0

  BMP280_REGISTER_CONTROL            = 0xF4,
  BMP280_REGISTER_CONFIG             = 0xF5,
  BMP280_REGISTER_PRESSUREDATA       = 0xF7,
  BMP280_REGISTER_TEMPDATA           = 0xFA,
};

bmp280_calib_data _bmp280_calib;

uint8_t read8(byte reg) {
  uint8_t value;
  Wire.beginTransmission((uint8_t)Bmp280Addr);
  Wire.write((uint8_t)reg);
  Wire.endTransmission();
  Wire.requestFrom((uint8_t)Bmp280Addr, (byte)1);
  value = Wire.read();
  return value;
}

void write8(byte reg, byte dat) {
  Wire.beginTransmission((uint8_t)Bmp280Addr);
  Wire.write((uint8_t)reg);
  Wire.write((uint8_t)dat);
  Wire.endTransmission();
}

uint16_t read16(byte reg) {
  uint16_t value;
  Wire.beginTransmission((uint8_t)Bmp280Addr);
  Wire.write((uint8_t)reg);
  Wire.endTransmission();
  Wire.requestFrom((uint8_t)Bmp280Addr, (byte)2);
  value = (Wire.read() << 8) | Wire.read();
  return value;
}

uint16_t read16_LE(byte reg) {
  uint16_t temp = read16(reg);
  return (temp >> 8) | (temp << 8);

}

int16_t readS16(byte reg)
{
  return (int16_t)read16(reg);

}

int16_t readS16_LE(byte reg)
{
  return (int16_t)read16_LE(reg);

}

uint32_t read24(byte reg) {
  uint32_t value;
  Wire.beginTransmission((uint8_t)Bmp280Addr);
  Wire.write((uint8_t)reg);
  Wire.endTransmission();
  Wire.requestFrom((uint8_t)Bmp280Addr, (byte)3);
  value = Wire.read();
  value <<= 8;
  value |= Wire.read();
  value <<= 8;
  value |= Wire.read();
  return value;

}

void readCoefficients(void)
{
  _bmp280_calib.dig_T1 = read16_LE(BMP280_REGISTER_DIG_T1);
  _bmp280_calib.dig_T2 = readS16_LE(BMP280_REGISTER_DIG_T2);
  _bmp280_calib.dig_T3 = readS16_LE(BMP280_REGISTER_DIG_T3);

  _bmp280_calib.dig_P1 = read16_LE(BMP280_REGISTER_DIG_P1);
  _bmp280_calib.dig_P2 = readS16_LE(BMP280_REGISTER_DIG_P2);
  _bmp280_calib.dig_P3 = readS16_LE(BMP280_REGISTER_DIG_P3);
  _bmp280_calib.dig_P4 = readS16_LE(BMP280_REGISTER_DIG_P4);
  _bmp280_calib.dig_P5 = readS16_LE(BMP280_REGISTER_DIG_P5);
  _bmp280_calib.dig_P6 = readS16_LE(BMP280_REGISTER_DIG_P6);
  _bmp280_calib.dig_P7 = readS16_LE(BMP280_REGISTER_DIG_P7);
  _bmp280_calib.dig_P8 = readS16_LE(BMP280_REGISTER_DIG_P8);
  _bmp280_calib.dig_P9 = readS16_LE(BMP280_REGISTER_DIG_P9);
}

float BMP280_readTemperature(void)
{
  int32_t var1, var2;

  int32_t adc_T = read24(0xFA);
  adc_T >>= 4;

  var1  = ((((adc_T >> 3) - ((int32_t)_bmp280_calib.dig_T1 << 1))) *
           ((int32_t)_bmp280_calib.dig_T2)) >> 11;

  var2  = (((((adc_T >> 4) - ((int32_t)_bmp280_calib.dig_T1)) *
             ((adc_T >> 4) - ((int32_t)_bmp280_calib.dig_T1))) >> 12) *
           ((int32_t)_bmp280_calib.dig_T3)) >> 14;

  t_fine = var1 + var2;

  float T  = (t_fine * 5 + 128) >> 8;
  return T / 100;
}

float BMP280_readPressure(void) {
  int64_t var1, var2, p;

  // Must be done first to get the t_fine variable set up
  BMP280_readTemperature();

  int32_t adc_P = read24(0xF7);
  adc_P >>= 4;

  var1 = ((int64_t)t_fine) - 128000;
  var2 = var1 * var1 * (int64_t)_bmp280_calib.dig_P6;
  var2 = var2 + ((var1 * (int64_t)_bmp280_calib.dig_P5) << 17);
  var2 = var2 + (((int64_t)_bmp280_calib.dig_P4) << 35);
  var1 = ((var1 * var1 * (int64_t)_bmp280_calib.dig_P3) >> 8) +
         ((var1 * (int64_t)_bmp280_calib.dig_P2) << 12);
  var1 = (((((int64_t)1) << 47) + var1)) * ((int64_t)_bmp280_calib.dig_P1) >> 33;

  if (var1 == 0) {
    return 0;  // avoid exception caused by division by zero
  }
  p = 1048576 - adc_P;
  p = (((p << 31) - var2) * 3125) / var1;
  var1 = (((int64_t)_bmp280_calib.dig_P9) * (p >> 13) * (p >> 13)) >> 25;
  var2 = (((int64_t)_bmp280_calib.dig_P8) * p) >> 19;

  p = ((p + var1 + var2) >> 8) + (((int64_t)_bmp280_calib.dig_P7) << 4);
  return (float)p / 256;
}

float BMP280_readAltitude() {
  float altitude;
  float pressure = BMP280_readPressure(); // in Si units for Pascal
  pressure /= 100;

  altitude = 44330 * (1.0 - pow(pressure / 1013.25, 0.1903));

  return altitude;
}


bool BMP280_init(void) {
  Wire.begin(2, 0);
  if (read8(0xD0) != 0x58)
    return false;
  readCoefficients();
  write8(0xF4, 0x3F);
  return true;
}

void Print_BMP280Data(void) {
  if (BMP280_init()) {
    Serial.print("BMP280 Temperature - "); Serial.println(BMP280_readTemperature());
    Serial.print("BMP280 Pressure - "); Serial.println(BMP280_readPressure());
    Serial.print("BMP280 Altitude - "); Serial.println(BMP280_readAltitude());
    BMP280  = "{";
    BMP280 += "\"BMP280_Temperature\":\"";
    BMP280 += BMP280_readTemperature();
    BMP280 += "\",\"BMP280_Pressure\":\"";
    BMP280 += BMP280_readPressure();
    BMP280 += "\",\"BMP280_Altitude\":\"";
    BMP280 += BMP280_readAltitude();
    BMP280 += "\"}";

  }
}
