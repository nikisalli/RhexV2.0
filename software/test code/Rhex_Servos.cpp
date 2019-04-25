#include <Servo.h>
#include "Rhex_Servos.h"
#include "Rhex_defines.h"
#include "Rhex_Hexapod.h"
#include "Rhex_math.h"
#include "Rhex_structs.h"

Servo Servos[18];

extern struct Dleg Dlegs[6];

void servoInit(){
  for (int i = 0; i < 18; i++) {
    Servos[i].attach(SERVO_PINS[i]);
  }
}

void servoDetach(){
  for (int i = 0; i < 18; i++) {
    Servos[i].detach();
  }
}

bool bodyToServos(float pos[][6]){
  float lol[18];
  for(int i=0;i<3;i++){
     trigz(lol[3*i],lol[3*i+1],lol[3*i+2],pos[0][i]-LEG_START_OFFSET[i*2],-(pos[1][i]-LEG_START_OFFSET[i*2+1]),pos[2][i],Dlegs[i]);
  }
  for(int i=3;i<6;i++){
     trigz(lol[3*i],lol[3*i+1],lol[3*i+2],pos[0][i]-LEG_START_OFFSET[i*2],pos[1][i]-LEG_START_OFFSET[i*2+1],pos[2][i],Dlegs[i]);
  }
  servoWrite(lol);
}

bool servoWrite(float angles[]){
  /*for (int i = 0; i < 18; i++) {
    if(angles[i]>180 or angles[i]<0){
      return 1;
    }
  }*/
  for (int i = 0; i < 18; i++) {
    Servos[i].write(limit(((int)angles[i]+SERVO_OFFSETS[i]),0,180));
  }
}


