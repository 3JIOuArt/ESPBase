void i2c_scanner()
{
  i2cStr = "";
  Wire.begin(2, 0);

  while (!Serial) // Waiting for serial connection
    {
    }

  Serial.println ();
  Serial.println ("I2C scanner. Scanning ...");
  byte count = 0;
  
  for (byte i = 8; i < 120; i++)
  {
    Wire.beginTransmission (i);
    if (Wire.endTransmission () == 0)
      {
      Serial.print ("Found address: ");
      Serial.print (i, DEC);
      Serial.print (" (0x");
      Serial.print (i, HEX);
      Serial.println (")");
      i2cStr  = "{";
      i2cStr +="\"Address\":\"";
      i2cStr +=" 0x";
      i2cStr += String(i, HEX);
      count++;
      delay (1);  // maybe unneeded?
      } // end of good response
  } // end of for loop
      i2cStr +="\",\"Found\":\"";
      i2cStr +=count, DEC;
      i2cStr +="\"}";
  Serial.println ("Done.");
  Serial.print ("Found ");
  Serial.print (count, DEC);
  Serial.println (" device(s).");
  BMP280_init();
}
