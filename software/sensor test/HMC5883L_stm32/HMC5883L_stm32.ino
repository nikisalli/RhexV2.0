#include <Wire.h> //I2C Arduino Library

TwoWire HWire(PB11,PB10);
HardwareSerial Serial1(PA10,PA9);

#define addr 0x1E //I2C Address for The HMC5883

void setup(){
  
  Serial1.begin(115200);
 HWire.begin();
  
  
 HWire.beginTransmission(addr); //start talking
 HWire.write(0x02); // Set the Register
 HWire.write(0x00); // Tell the HMC5883 to Continuously Measure
 HWire.endTransmission();
}


void loop(){
  
  int16_t x,y,z; //triple axis data

  //Tell the HMC what regist to begin writing data into
 HWire.beginTransmission(addr);
 HWire.write(0x03); //start with register 3.
 HWire.endTransmission();
  
 
 //Read the data.. 2 bytes for each axis.. 6 total bytes
 HWire.requestFrom(addr, 6);
  if(6<=Wire.available()){
    x =HWire.read()<<8; //MSB  x 
    x |=HWire.read(); //LSB  x
    z =HWire.read()<<8; //MSB  z
    z |=HWire.read(); //LSB z
    y =HWire.read()<<8; //MSB y
    y |=HWire.read(); //LSB y
  }
  
  // Show Values
  Serial1.print("X: ");
  Serial1.print(x);
  Serial1.print("  Y: ");
  Serial1.print(y);
  Serial1.print("  Z: ");
  Serial1.print(z);
  Serial1.println();
  
  delay(10);
}
