/* This file is part of the Pluviam Firmware */

// copyright, Peter H Anderson, Baltimore, MD, Nov, '11
// You may use it, but please give credit.  

// created by Saulo Matte Madalozzo (saulo.zz@gmail.com) Oct, 2013

float temperature, humidity;

float getTemperature(){
  return temperature;
}

float getHumidity(){
  return humidity;
}

void getHIH6130Data(void)
{
  unsigned int H_dat, T_dat;
  fetch_humidity_temperature(&H_dat, &T_dat); 
  humidity = (float) H_dat * 6.10e-3;
  temperature = (float) T_dat * 1.007e-2 - 40.0;
}

void fetch_humidity_temperature(unsigned int *p_H_dat, unsigned int *p_T_dat)
{
      byte address, Hum_H, Hum_L, Temp_H, Temp_L;
      unsigned int H_dat, T_dat;
      address = 0x27;;
      Wire.beginTransmission(address); 
      Wire.endTransmission();
      delay(100);
      
      Wire.requestFrom((int)address, (int) 4);
      Hum_H = Wire.read();
      Hum_L = Wire.read();
      Temp_H = Wire.read();
      Temp_L = Wire.read();
      Wire.endTransmission();
      
      //_status = (Hum_H >> 6) & 0x03;
      Hum_H = Hum_H & 0x3f;
      H_dat = (((unsigned int)Hum_H) << 8) | Hum_L;
      T_dat = (((unsigned int)Temp_H) << 8) | Temp_L;
      T_dat = T_dat / 4;
      *p_H_dat = H_dat;
      *p_T_dat = T_dat;
      //return(_status);
}
