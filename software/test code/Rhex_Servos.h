#ifndef RHEX_SERVOS_H
#define RHEX_SERVOS_H

void servoInit();
void servoDetach();
bool bodyToServos(float pos[][6]);
bool servoWrite(float angles[]);

#endif
