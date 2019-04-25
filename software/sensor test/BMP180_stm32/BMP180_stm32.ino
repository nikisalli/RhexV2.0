// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// BMP180
// This code is designed to work with the BMP180_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/content/Pressure?sku=BMP180_I2CS#tabs-0-product_tabset-2

#include <Wire.h>

TwoWire HWire(PB11,PB10);
HardwareSerial Serial1(PA10,PA9);
// BMP180 I2C address is 0x77(109)
#define Addr 0x77

int coff[11] = {0};
unsigned int AC4 = 0, AC5 = 0, AC6 = 0;
unsigned int data[3] = {0};

void setup()
{
  // Initialise I2C communication as MASTER
  HWire.begin();
  // Initialise serial communication, set baud rate = 9600
  Serial1.begin(9600);

  for (int i = 0; i < 11; i++)
  {
    // Start I2C Transmission
    HWire.beginTransmission(Addr);
    // Select data register
    HWire.write((170 + (2 * i)));
    // Stop I2C Transmission
    HWire.endTransmission(true);

    // Request 2 bytes of data
    HWire.requestFrom(Addr, 2);

    // Read 2 bytes of data
    // coff msb, coff lsb
    if (HWire.available() == 2)
    {
      data[0] = HWire.read();
      data[1] = HWire.read();
    }

    delay(50);
    coff[i] = data[0] * 256 + data[1];
    if ((coff[i] > 32767) && (i != 3) && (i != 4) && (i != 5))
    {
      coff[i] -= 65536;
    }
  }
  AC4 = coff[3];
  AC5 = coff[4];
  AC6 = coff[5];
  delay(300);
}

void loop()
{
  // Start I2C Transmission
  HWire.beginTransmission(Addr);
  // Select measurement control register
  HWire.write(0xF4);
  // Enable temperature measurement
  HWire.write(0x2E);
  // Stop I2C Transmission
  HWire.endTransmission();
  delay(100);

  // Start I2C Transmission
  HWire.beginTransmission(Addr);
  // Select data register
  HWire.write(0xF6);
  // Stop I2C Transmission
  HWire.endTransmission();

  // Request 2 bytes of data
  HWire.requestFrom(Addr, 2);

  // Read 2 bytes of data
  // temp msb, temp lsb
  if (HWire.available() == 2)
  {
    data[0] = HWire.read();
    data[1] = HWire.read();
  }

  // Convert the data
  float temp = ((data[0] * 256.0) + data[1]);

  // Start I2C Transmission
  HWire.beginTransmission(Addr);
  // Select measurement control register
  HWire.write(0xF4);
  // Enable pressure measurement, OSS = 1
  HWire.write(0x74);
  // Stop I2C Transmission
  HWire.endTransmission();
  delay(100);

  // Start I2C Transmission
  HWire.beginTransmission(Addr);
  // Select data register
  HWire.write(0xF6);
  // Stop I2C Transmission
  HWire.endTransmission();
  // Request 3 bytes of data
  HWire.requestFrom(Addr, 3);

  // Read 3 bytes of data
  // pres msb1, pres msb, pres lsb
  if (HWire.available() == 3)
  {
    data[0] = HWire.read();
    data[1] = HWire.read();
    data[2] = HWire.read();
  }

  // Convert the data
  long long pres = (((long long)data[0] * (long long)65536) + ((long)data[1] * 256) + data[2]) / 128;

  // Callibration for Temperature
  double X1 = (temp - AC6) * AC5 / 32768.0;
  double X2 = ((double)coff[9] * 2048.0) / (X1 + coff[10]);
  double B5 = X1 + X2;
  float cTemp = ((B5 + 8.0) / 16.0) / 10.0;
  float fTemp = cTemp * 1.8 + 32;

  // Calibration for Pressure
  double B6 = B5 - 4000;
  X1 = (coff[7] * ((long)B6 * (long)B6 / 4096.0)) / 2048.0;
  X2 = coff[1] * B6 / 2048.0;
  double X3 = X1 + X2;
  double B3 = (((coff[0] * 4 + X3) * 2) + 2) / 4.0;
  X1 = coff[2] * B6 / 8192.0;
  X2 = (coff[6] * ((long)B6 * (long)B6 / 2048.0)) / 65536.0;
  X3 = ((X1 + X2) + 2) / 4.0;
  double B4 = AC4 * (X3 + 32768) / 32768.0;
  long long B7 = (((unsigned long long )pres - (unsigned long long )B3) * ((unsigned long long )25000.0));
  float pressure = 0.0;
  if (B7 < 2147483648)
  {
    pressure = (B7 * 2) / B4;
  }
  else
  {
    pressure = (B7 / B4) * 2;
  }
  X1 = (pressure / 256.0) * (pressure / 256.0);
  X1 = (X1 * 3038.0) / 65536.0;
  X2 = ((-7357) * pressure) / 65536.0;
  pressure = (pressure + (X1 + X2 + 3791) / 16.0) / 100;

  // Calculate Altitude
  float height = 44330 * (1 - pow((pressure / 1013.25), 0.1903));

  // Output data to serial monitor
  Serial1.print("Altitude : ");
  Serial1.print(height);
  Serial1.println(" m");
  Serial1.print("Pressure : ");
  Serial1.print(pressure);
  Serial1.println(" hPa");
  Serial1.print("Temperature in Celsius : ");
  Serial1.print(cTemp);
  Serial1.println(" C");
  Serial1.print("Temperature in Fahrenheit : ");
  Serial1.print(fTemp);
  Serial1.println(" F");
  delay(500);
}
