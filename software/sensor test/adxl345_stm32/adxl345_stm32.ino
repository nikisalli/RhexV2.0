#include<Wire.h>

TwoWire HWire(PB11,PB10);
HardwareSerial Serial1(PA10,PA9);
#define Addr 0x53
void setup()
{
  // Initialise I2C communication as MASTER
  HWire.begin();
  // Initialise serial communication, set baud rate = 9600
  Serial1.begin(115200);
  // Start I2C Transmission
  HWire.beginTransmission(Addr);
  // Select bandwidth rate register
  HWire.write(0x2C);
  // Normal mode, Output data rate = 100 Hz
  HWire.write(0x0A);
  // Stop I2C transmission
  HWire.endTransmission();
  // Start I2C Transmission
  HWire.beginTransmission(Addr);
  // Select power control register
  HWire.write(0x2D);
  // Auto-sleep disable
  HWire.write(0x08);
  // Stop I2C transmission
  HWire.endTransmission();
  // Start I2C Transmission
  HWire.beginTransmission(Addr);
  // Select data format register
  HWire.write(0x31);
  // Self test disabled, 4-wire interface, Full resolution, Range = +/-2g
  HWire.write(0x08);
  // Stop I2C transmission
  HWire.endTransmission();
  delay(300);
}
void loop()
{
  unsigned int data[6];
  for (int i = 0; i < 6; i++)
  {
    // Start I2C Transmission
    HWire.beginTransmission(Addr);
    // Select data register
    HWire.write((50 + i));
    // Stop I2C transmission
    HWire.endTransmission();
    // Request 1 byte of data
    HWire.requestFrom(Addr, 1);
    // Read 6 bytes of data
    // xAccl lsb, xAccl msb, yAccl lsb, yAccl msb, zAccl lsb, zAccl msb
    if (Wire.available() == 1)
    {
      data[i] = HWire.read();
    }
  }
  // Convert the data to 10-bits
  int xAccl = (((data[1] & 0x03) * 256) + data[0]);
  if (xAccl > 511)
  {
    xAccl -= 1024;
  }
  int yAccl = (((data[3] & 0x03) * 256) + data[2]);
  if (yAccl > 511)
  {
    yAccl -= 1024;
  }
  int zAccl = (((data[5] & 0x03) * 256) + data[4]);
  if (zAccl > 511)
  {
    zAccl -= 1024;
  }
  // Output data to serial monitor
  Serial1.print("X: ");
  Serial1.print(xAccl);
  Serial1.print("  Y: ");
  Serial1.print(yAccl);
  Serial1.print("  Z: ");
  Serial1.println(zAccl);
  delay(30);
}
