#include <Wire.h>
#include "Rhex_sensors.h"
#include "Rhex_defines.h"
#include "Rhex_math.h"

TwoWire HWire(SENSORS_SDA, SENSORS_SCL);

sensors::sensors(){
}

void sensors::init(){
  HWire.begin();
  HWire.beginTransmission(L3G4200D_ADDRESS); 
  HWire.write(CTRL_REG1);      
  HWire.write(0b00001111);         
  HWire.endTransmission(); 
  HWire.beginTransmission(L3G4200D_ADDRESS); 
  HWire.write(CTRL_REG2);      
  HWire.write(0b00000000);         
  HWire.endTransmission(); 
  HWire.beginTransmission(L3G4200D_ADDRESS); 
  HWire.write(CTRL_REG3);      
  HWire.write(0b00001000);         
  HWire.endTransmission(); 
  HWire.beginTransmission(L3G4200D_ADDRESS); 
  HWire.write(CTRL_REG4);      
  HWire.write(0b00000000);         
  HWire.endTransmission(); 
  HWire.beginTransmission(L3G4200D_ADDRESS); 
  HWire.write(CTRL_REG5);      
  HWire.write(0b00000000);         
  HWire.endTransmission(); 
  delay(300);
  HWire.beginTransmission(HMC5883L_ADDRESS); //start talking
  HWire.write(0x02); // Set the Register
  HWire.write(0x00); // Tell the HMC5883 to Continuously Measure
  HWire.endTransmission();
  delay(300);
  HWire.beginTransmission(ADXL345_ADDRESS);
  HWire.write(0x2C);
  HWire.write(0x0A);
  HWire.endTransmission();
  HWire.beginTransmission(ADXL345_ADDRESS);
  HWire.write(0x2D);
  HWire.write(0x08);
  HWire.endTransmission();
  HWire.beginTransmission(ADXL345_ADDRESS);
  HWire.write(0x31);
  HWire.write(0x08);
  HWire.endTransmission();
}

void sensors::ADXL345(int buf[]){
  unsigned int data[6];
  for (int i = 0; i < 6; i++){
    HWire.beginTransmission(ADXL345_ADDRESS);
    HWire.write((50 + i));
    HWire.endTransmission();
    HWire.requestFrom(ADXL345_ADDRESS, 1);
    if (Wire.available() == 1){
      data[i] = HWire.read();
    }
  }
  buf[0] = (((data[1] & 0x03) * 256) + data[0]);
  if (buf[0] > 511){
    buf[0] -= 1024;
  }
  buf[0]=map(buf[0],-260,260,-90,90);
  buf[1] = (((data[3] & 0x03) * 256) + data[2]);
  if (buf[1] > 511){
    buf[1] -= 1024;
  }
  buf[1]=map(buf[1],-260,260,-90,90);
  buf[2] = (((data[5] & 0x03) * 256) + data[4]);
  if (buf[2] > 511){
    buf[2] -= 1024;
  }
  buf[2]=map(buf[2],-260,260,-90,90);
}

void sensors::HMC5883L(int buf[]){
  HWire.beginTransmission(HMC5883L_ADDRESS);
  HWire.write(0x03); //start with register 3.
  HWire.endTransmission();
  HWire.requestFrom(HMC5883L_ADDRESS, 6);
  int16_t x = HWire.read()<<8|HWire.read();
  int16_t y = HWire.read()<<8|HWire.read();
  int16_t z = HWire.read()<<8|HWire.read();
  buf[6] = x;
  buf[7] = y;
  buf[8] = z;
}

void sensors::L3G4200D(int buf[]){
  int16_t x,y,z;
  HWire.beginTransmission(L3G4200D_ADDRESS);
  HWire.write(0x29); // register to read
  HWire.endTransmission();
  HWire.requestFrom(L3G4200D_ADDRESS, 1); // read a byte
  byte xMSB = HWire.read();
  HWire.beginTransmission(L3G4200D_ADDRESS);
  HWire.write(0x28); // register to read
  HWire.endTransmission();
  HWire.requestFrom(L3G4200D_ADDRESS, 1); // read a byte
  byte xLSB = HWire.read();
  x = ((xMSB << 8) | xLSB);
  buf[3] = x;

  HWire.beginTransmission(L3G4200D_ADDRESS);
  HWire.write(0x2B); // register to read
  HWire.endTransmission();
  HWire.requestFrom(L3G4200D_ADDRESS, 1); // read a byte
  byte yMSB = HWire.read();
  HWire.beginTransmission(L3G4200D_ADDRESS);
  HWire.write(0x2A); // register to read
  HWire.endTransmission();
  HWire.requestFrom(L3G4200D_ADDRESS, 1); // read a byte
  byte yLSB = HWire.read();
  y = ((yMSB << 8) | yLSB);
  buf[4] = y;

  HWire.beginTransmission(L3G4200D_ADDRESS);
  HWire.write(0x2D); // register to read
  HWire.endTransmission();
  HWire.requestFrom(L3G4200D_ADDRESS, 1); // read a byte
  byte zMSB = HWire.read();
  HWire.beginTransmission(L3G4200D_ADDRESS);
  HWire.write(0x2C); // register to read
  HWire.endTransmission();
  HWire.requestFrom(L3G4200D_ADDRESS, 1); // read a byte
  byte zLSB = HWire.read();
  z = ((zMSB << 8) | zLSB);
  buf[5] = z;
}
