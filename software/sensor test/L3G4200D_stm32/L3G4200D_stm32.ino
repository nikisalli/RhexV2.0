//Arduino 1.0+ only

#include <Wire.h>

TwoWire HWire(PB11,PB10);
HardwareSerial Serial1(PA10,PA9);

#define CTRL_REG1 0x20
#define CTRL_REG2 0x21
#define CTRL_REG3 0x22
#define CTRL_REG4 0x23
#define CTRL_REG5 0x24

int L3G4200D_Address = 105; //I2C address of the L3G4200D

int16_t x;
int16_t y;
int16_t z;

void setup(){

  HWire.begin();
  Serial1.begin(115200);

  Serial1.println("starting up L3G4200D");
  setupL3G4200D(250); // Configure L3G4200  - 250, 500 or 2000 deg/sec

  delay(1500); //wait for the sensor to be ready 
}

void loop(){
   getGyroValues();  // This will update x, y, and z with new values

  Serial1.print("X:");
  Serial1.print(x);
  Serial1.print(" Y:");
  Serial1.print(y);
  Serial1.print(" Z:");
  Serial1.println(z);

  delay(10); //Just here to slow down the Serial1 to make it more readable
}

void getGyroValues(){

  byte xMSB = readRegister(L3G4200D_Address, 0x29);
  byte xLSB = readRegister(L3G4200D_Address, 0x28);
  x = ((xMSB << 8) | xLSB);

  byte yMSB = readRegister(L3G4200D_Address, 0x2B);
  byte yLSB = readRegister(L3G4200D_Address, 0x2A);
  y = ((yMSB << 8) | yLSB);

  byte zMSB = readRegister(L3G4200D_Address, 0x2D);
  byte zLSB = readRegister(L3G4200D_Address, 0x2C);
  z = ((zMSB << 8) | zLSB);
}

int setupL3G4200D(int scale){
  //From  Jim Lindblom of Sparkfun's code

  // Enable x, y, z and turn off power down:
  writeRegister(L3G4200D_Address, CTRL_REG1, 0b00001111);

  // If you'd like to adjust/use the HPF, you can edit the line below to configure CTRL_REG2:
  writeRegister(L3G4200D_Address, CTRL_REG2, 0b00000000);

  // Configure CTRL_REG3 to generate data ready interrupt on INT2
  // No interrupts used on INT1, if you'd like to configure INT1
  // or INT2 otherwise, consult the datasheet:
  writeRegister(L3G4200D_Address, CTRL_REG3, 0b00001000);

  // CTRL_REG4 controls the full-scale range, among other things:

  if(scale == 250){
    writeRegister(L3G4200D_Address, CTRL_REG4, 0b00000000);
  }else if(scale == 500){
    writeRegister(L3G4200D_Address, CTRL_REG4, 0b00010000);
  }else{
    writeRegister(L3G4200D_Address, CTRL_REG4, 0b00110000);
  }

  // CTRL_REG5 controls high-pass filtering of outputs, use it
  // if you'd like:
  writeRegister(L3G4200D_Address, CTRL_REG5, 0b00000000);
}

void writeRegister(int deviceAddress, byte address, byte val) {
    HWire.beginTransmission(deviceAddress); // start transmission to device 
    HWire.write(address);       // send register address
    HWire.write(val);         // send value to write
    HWire.endTransmission();     // end transmission
}

int readRegister(int deviceAddress, byte address){

    int v;
    HWire.beginTransmission(deviceAddress);
    HWire.write(address); // register to read
    HWire.endTransmission();

    HWire.requestFrom(deviceAddress, 1); // read a byte

   // <============================================I NEVER GET OUT OF THIS LOOP
    while(!HWire.available()) {
        // waiting    
    }

    v = HWire.read();
    return v;
}
