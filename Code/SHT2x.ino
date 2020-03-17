uint16_t readSensor(uint8_t command)
{
    uint16_t result;

    Wire.beginTransmission(0x40);
    Wire.write(command);
    delay(100);
    Wire.endTransmission();

    Wire.requestFrom(0x40, 3);
    uint32_t timeout = millis() + 300;       // Don't hang here for more than 300ms
    while (Wire.available() < 3) {
        if ((millis() - timeout) > 0) {
            return 0;
        }
    }

    //Store the result
    result = Wire.read() << 8;
    result += Wire.read();
    result &= ~0x0003;   // clear two low bits (status bits)

    //Clear the final byte from the buffer
    Wire.read();

    return result;
}

float GetHumidity(void)
{
    float value = readSensor(0xE5);
    if (value == 0) {
        return 0;                       // Some unrealistic value
    }
    return -6.0 + 125.0 / 65536.0 * value;
}

float GetTemperature(void)
{
    float value = readSensor(0xE3);
    if (value == 0) {
        return -273;                    // Roughly Zero Kelvin indicates an error
    }
    return -46.85 + 175.72 / 65536.0 * value;
}
