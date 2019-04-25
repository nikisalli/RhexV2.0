#include "Arduino.h"
#include "Rhex_defines.h"
#include "Rhex_Hexapod.h"
#include "Rhex_structs.h"
#include "Rhex_Servos.h"
#include "Rhex_math.h"
#include "Rhex_sensors.h"
#include "Rhex_serial.h"

struct Dleg Dlegs[6];
float pos[3][6] = {};
float buff1[4] = {};
byte leg;
unsigned long timing;

sensors sensors;

Hexapod::Hexapod(){
  for(int i=0;i<6;i++){
    Dlegs[i].CX_LENGHT = LEG_DIMENSIONS[i*3];
    Dlegs[i].FM_LENGHT = LEG_DIMENSIONS[i*3+1];
    Dlegs[i].TB_LENGHT = LEG_DIMENSIONS[i*3+2];
    Dlegs[i].START_X_OFFSET = LEG_START_OFFSET[i*2];
    Dlegs[i].START_Y_OFFSET = LEG_START_OFFSET[i*2+1];
    Dlegs[i].SIDE = LEG_SIDE[i];
  }
}

void Hexapod::init(){
  sensors.init();
  initSerialComm();
  initSerialComp();
}

void Hexapod::standUp(){
  servoInit();
  for(float j=0;j<1;j+=0.01){
    for(int i=0;i<6;i++){
      pos[0][i] = STAND_POS[0][i];
      pos[1][i] = STAND_POS[1][i];
      pos[2][i] = timeVal(j,STAND_Z,START_Z);
    }
    bodyToServos(pos);
    delay(25);
  }
}

void Hexapod::stand(){
  if(!(compareMatrix(pos,STAND_POS))){
    int z = millis() % 10;
    for(int k=0;k<6;k++){
      int j = RANDOM_SEQ[z][k];
      float x,y,z;
      float p[3][4] = {
        {STAND_POS[0][j],STAND_POS[0][j],pos[0][j],pos[0][j]},
        {STAND_POS[1][j],STAND_POS[1][j],pos[1][j],pos[1][j]},
        {STAND_POS[2][j],STAND_POS[2][j]+4,pos[2][j]+4,pos[2][j]},
      };
      for(float i=0;i<=1;i+=0.01){
        Spline3D(p,i,pos[0][j],pos[1][j],pos[2][j]);
        bodyToServos(pos);
        delay(3);
      }
      pos[0][j] = STAND_POS[0][j];
      pos[1][j] = STAND_POS[1][j];
      pos[2][j] = STAND_POS[2][j];
    }
  }
  else{
    bodyToServos(pos);
  }
}

void Hexapod::step(int dir[]){
  float cx,cy,angle;
  float arc = limit(fmap(sqrt(sq(dir[2]) + sq(dir[3])),0,500,0,8),0,8);
  if(dir[2]>0){
    angle = toDeg(atan2(dir[3]/500.0, dir[2]/500.0));
    cy = ((1/abs(angle))*600.0)-5.0;
  }
  else{
    angle = toDeg(atan2(-dir[3]/500.0, -dir[2]/500.0));
    cy = (((1/abs(angle))*600.0)-5.0);
  }
  if(dir[3]<0){
    cy = -cy;
    arc = -arc;
  }
  for(float i=0;i<1;i+=0.01){
    stepCalc(0,i,0,cy,6,pos,arc);
    bodyToServos(pos);
    delay(map(abs(arc),0,8,20,3));
  }
}

void Hexapod::moveleg(int dir[]){
  if(dir[1]>400 and (millis()-timing)>300){
    timing = millis();
    leg++;
    if(leg > 5){
      leg = 0;
    }
  }
  else if(dir[1]<-400 and (millis()-timing)>300){
    timing = millis();
    leg--;
    if(leg < 0){
      leg = 5;
    }
  }
  float actualpos[3][6] = {}; 
  for(int i=0;i<6;i++){
    for(int j=0;j<3;j++){
      actualpos[j][i] = STAND_POS[j][i];
      pos[j][i] = STAND_POS[j][i];
    }
  }
  actualpos[0][leg] += limit(map(dir[3],-500,500,-6,6),-6,6);
  actualpos[1][leg] += limit(map(dir[2],-500,500,-6,6),-6,6);
  actualpos[2][leg] += limit(map(dir[0],-500,500,12,0),-6,6);
  pos[0][leg] = actualpos[0][leg];
  pos[1][leg] = actualpos[1][leg];
  pos[2][leg] = actualpos[2][leg];
  bodyToServos(pos);
  delay(20);
}

void Hexapod::move(int dir1[]){
  int dir[4] = {};
  for(int i=0;i<4;i++){
    dir[i] = dir1[i];
  }
  for(int i=0;i<4;i++){
    buff1[i] = (dir[i]*(1-0.96))+(buff1[i]*0.96);
  }
  for(int i=0;i<6;i++){
    float actualpos[3][6] = {};
    for(int j=0;j<3;j++){
      actualpos[j][i] = STAND_POS[j][i];
    }
    Rmatrix(actualpos[0][i],actualpos[1][i],actualpos[2][i],limit(fmap(buff1[1],-500,500,-15,15),-15,15),limit(fmap(buff1[0],-500,500,-15,15),-15,15),0,0,0,0);
    actualpos[0][i] += limit(fmap(buff1[2],-500,500,8,-8),-8,8);
    actualpos[1][i] += limit(fmap(buff1[3],-500,500,8,-8),-8,8);
    for(int j=0;j<3;j++){
      pos[j][i] = actualpos[j][i];
    }
  }
  bodyToServos(pos);
  delay(20);
}

void stepCalc(int walk_pattern, float t, int Cx, int Cy, float height, float actualpos[][6], float arc){
  float ray = sqrt(sq(Cy) + sq(Cx));
  float walkcirc = 2 * PI * (ray + STAND_POS[1][4]);
  float ang = ((arc * 360.0) / walkcirc)*2;
  float A,B;
  float p1[2][11] = {
    {0,-((ang/2)*0.25),-((ang/2)*0.5),-((ang/2)*0.75),-(ang/2),0,(ang/2),((ang/2)*0.75),((ang/2)*0.5),((ang/2)*0.25),0},
    {STAND_Z,STAND_Z,STAND_Z,STAND_Z,STAND_Z,height,STAND_Z,STAND_Z,STAND_Z,STAND_Z,STAND_Z},
  };
  for(int i=0;i<6;i++){
    actualpos[0][i] = STAND_POS[0][i];
    actualpos[1][i] = STAND_POS[1][i];
    actualpos[2][i] = STAND_POS[2][i];
  }
  if(walk_pattern == 0){
    for(int i=0;i<6;i+=2){
      Spline2D(p1,t,A,B);
      RmatrixZ(actualpos[0][i],actualpos[1][i],actualpos[2][i],A,Cx,Cy,0);
      actualpos[2][i] = B;
      float t1 = t+0.5;
      if(t1>1){
        t1=t1-1.0;
      }
      Spline2D(p1,t1,A,B);
      RmatrixZ(actualpos[0][i+1],actualpos[1][i+1],actualpos[2][i+1],A,Cx,Cy,0);
      actualpos[2][i+1] = B;
    }
  }
}

