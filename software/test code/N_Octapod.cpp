#include "N_upload.h"
#include "Arduino.h"
#include "N_defines.h"
#include "N_Octapod.h"
#include "N_structs.h"
#include "N_Servos.h"
#include "N_math.h"

struct Dleg Dlegs[8];
float pos[3][8] = {};
float bpos[3][2] = {};
boolean first_step = true;
byte leg;

Octapod::Octapod() {
  for (int i = 0; i < 8; i++) {
    Dlegs[i].CX_LENGHT = LEG_DIMENSIONS[i * 3];
    Dlegs[i].FM_LENGHT = LEG_DIMENSIONS[i * 3 + 1];
    Dlegs[i].TB_LENGHT = LEG_DIMENSIONS[i * 3 + 2];
    Dlegs[i].START_X_OFFSET = LEG_START_OFFSET[i * 2];
    Dlegs[i].START_Y_OFFSET = LEG_START_OFFSET[i * 2 + 1];
    Dlegs[i].SIDE = LEG_SIDE[i];
  }
}

void Octapod::init() {
  esp32server_setup();

  pinMode(21, OUTPUT);
  pinMode(22, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);

  digitalWrite(21, HIGH);
  digitalWrite(22, LOW);

  servoInit();
}

void Octapod::standUp() {
  bodyAttach();
  delay(5);
  for (float j = 0; j < 1; j += 0.01) {
    for (int i = 0; i < 8; i++) {
      pos[0][i] = STAND_POS[0][i];
      pos[1][i] = STAND_POS[1][i];
      pos[2][i] = timeVal(j, STAND_Z, START_Z);
    }
    bodyToServos(pos);
    delay(20);
  }
}

void Octapod::sit() {
  for (float j = 0; j < 1; j += 0.01) {
    for (int i = 0; i < 8; i++) {
      pos[0][i] = STAND_POS[0][i];
      pos[1][i] = STAND_POS[1][i];
      pos[2][i] = timeVal(j, START_Z, STAND_Z);
    }
    bodyToServos(pos);
    delay(20);
  }
  bodyDetach();
}

void Octapod::stand() {
  first_step = true;
  if (!(compareMatrix(pos, STAND_POS))) {
    int l = millis() % 2;
    for (int k = 0; k < 4; k++) {
      int j = RANDOM_SEQ[l][k];
      int q = RANDOM_SEQ[l][k + 4];
      float p[3][4] = {
        {STAND_POS[0][j], STAND_POS[0][j], pos[0][j], pos[0][j]},
        {STAND_POS[1][j], STAND_POS[1][j], pos[1][j], pos[1][j]},
        {STAND_POS[2][j], STAND_POS[2][j] + 4, pos[2][j] + 4, pos[2][j]},
      };
      float t[3][4] = {
        {STAND_POS[0][q], STAND_POS[0][q], pos[0][q], pos[0][q]},
        {STAND_POS[1][q], STAND_POS[1][q], pos[1][q], pos[1][q]},
        {STAND_POS[2][q], STAND_POS[2][q] + 4, pos[2][q] + 4, pos[2][q]},
      };
      for (float i = 0; i <= 1; i += 0.01) {
        Spline3D(p, i, pos[0][j], pos[1][j], pos[2][j]);
        Spline3D(t, i, pos[0][q], pos[1][q], pos[2][q]);
        bodyToServos(pos);
        delay(3);
      }
      pos[0][j] = STAND_POS[0][j];
      pos[1][j] = STAND_POS[1][j];
      pos[2][j] = STAND_POS[2][j];
      pos[0][q] = STAND_POS[0][q];
      pos[1][q] = STAND_POS[1][q];
      pos[2][q] = STAND_POS[2][q];
    }
  }
  else {
    bodyToServos(pos);
  }
}

void Octapod::force_stand() {
  first_step = true;
  for (int i = 0; i < 8; i++) {
    pos[0][i] = STAND_POS[0][i];
    pos[1][i] = STAND_POS[1][i];
    pos[2][i] = STAND_POS[2][i];
  }
  bodyToServos(pos);
}

void Octapod::step(int walk_pattern, float height, float time) {
  if (walk_pattern == 1) {
    int sequence[] = {4, 2, 6, 0, 3, 5, 1, 7};
    for (int a = 0; a < 8; a++) {
      int fz, pz;
      fz = sequence[a];
      if (a == 0) {
        pz = sequence[7];
      } else {
        pz = sequence[a - 1];
      }
      bpos[0][1] = bpos[0][0];//p
      bpos[1][1] = bpos[1][0];//p
      bpos[2][1] = bpos[2][0];//p
      bpos[0][0] = pos[0][fz];//f
      bpos[1][0] = pos[1][fz];//f
      bpos[2][0] = pos[2][fz];//f
      for (float b = 0; b < 1; b += 0.05) {
        for (int c = 0; c < 8; c++) {
          if (!first_step) {
            if (c != fz && c != pz) {
              RmatrixZ(pos[0][c], pos[1][c], pos[2][c], -(Octapod::walk_ang / 120), Octapod::Cx, Octapod::Cy, 0);
            }
          } else {
            if (c != fz) {
              RmatrixZ(pos[0][c], pos[1][c], pos[2][c], -(Octapod::walk_ang / 120), Octapod::Cx, Octapod::Cy, 0);
            }
          }
        }
        if (!first_step) {
          float cr[3] = {STAND_POS[0][pz], STAND_POS[1][pz], STAND_POS[2][pz]};
          RmatrixZ(cr[0], cr[1], cr[2], Octapod::walk_ang / 2.0, Octapod::Cx, Octapod::Cy, 0); //prev
          float bm[3][4] = {//prev
            {cr[0], cr[0], bpos[0][1], bpos[0][1]},
            {cr[1], cr[1], bpos[1][1], bpos[1][1]},
            {STAND_POS[2][pz], STAND_POS[2][pz] + height, STAND_POS[2][pz] + height, STAND_POS[2][pz]},
          };
          Spline3D(bm, (b / 2.0) + 0.5, pos[0][pz], pos[1][pz], pos[2][pz]); //prev
        }
        float br[3] = {STAND_POS[0][fz], STAND_POS[1][fz], STAND_POS[2][fz]};
        RmatrixZ(br[0], br[1], br[2], Octapod::walk_ang / 2.0, Octapod::Cx, Octapod::Cy, 0); //forw
        float am[3][4] = {//forw
          {br[0], br[0], bpos[0][0], bpos[0][0]},
          {br[1], br[1], bpos[1][0], bpos[1][0]},
          {STAND_POS[2][fz], STAND_POS[2][fz] + height, STAND_POS[2][fz] + height, STAND_POS[2][fz]},
        };
        Spline3D(am, b / 2.0, pos[0][fz], pos[1][fz], pos[2][fz]); //forw

        bodyToServos(pos);
        delayMicroseconds(time * 6250);
      }
      first_step = false;
    }
  }
}

void Octapod::moveleg() {
  /*if(dir[1]>400 and (millis()-timing)>300){
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
    float actualpos[3][8] = {};
    for(int i=0;i<8;i++){
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
    delay(20);*/
}

void Octapod::move(float alpha, float beta, float gamma, float x_move, float y_move, float z_move) {
  for (int i = 0; i < 8; i++) {
    float actualpos[3][8] = {};
    for (int j = 0; j < 3; j++) {
      actualpos[j][i] = STAND_POS[j][i];
    }
    Rmatrix(actualpos[0][i], actualpos[1][i], actualpos[2][i], alpha, beta, gamma, 0, 0, 0);
    actualpos[0][i] += x_move;
    actualpos[1][i] += y_move;
    for (int j = 0; j < 3; j++) {
      pos[j][i] = actualpos[j][i];
    }
  }
  bodyToServos(pos);
}

