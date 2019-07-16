#ifndef N_HC12_H
#define N_HC12_H

#include "Arduino.h"

class remote{
  public:
    int angle1_;
    int speed1_;
    int angle2_;
    int speed2_;
    int active_;
    int state_;
    remote();
    void init();
    bool receive();
    void write();
};

#endif
