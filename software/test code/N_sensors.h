#ifndef N_SENSORS_H
#define N_SENSORS_H

#include "Arduino.h"

void sensors_init();
float read_battery_voltage();
float read_battery_current();

#endif
