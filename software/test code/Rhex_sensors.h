#ifndef RHEX_SENSORS_H
#define RHEX_SENSORS_H

class sensors{
  public:
    sensors();
    void init();
    void L3G4200D(int buf[]);
    void HMC5883L(int buf[]);
    void ADXL345(int buf[]);
};

#endif //RHEX_MPU6050_H
