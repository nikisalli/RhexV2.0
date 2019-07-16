#include "Arduino.h"
#include "N_math.h"
#include "N_defines.h"
#include "N_structs.h"

void trigz(float &gama, float &alpha, float &beta, float x, float y, float z, struct Dleg dim) {
  if (dim.SIDE) { //LEFT
    float L, L1, alpha1, alpha2;
    L1 = sqrt(sq(x) + sq(y));
    gama = - (atan(x / y) / PI * 180);
    L = sqrt(sq(L1 - dim.CX_LENGHT) + sq(-z));
    beta = - (acos((sq(dim.TB_LENGHT) + sq(dim.FM_LENGHT) - sq(L)) / (2 * dim.TB_LENGHT * dim.FM_LENGHT)) / PI * 180) + 180;
    alpha1 = acos(-z / L) / PI * 180;
    alpha2 = acos((sq(dim.FM_LENGHT) + sq(L) - sq(dim.TB_LENGHT)) / (2 * dim.FM_LENGHT * L)) / PI * 180;
    alpha = - (alpha1 + alpha2) + 90;
  }
  else {          //RIGHT
    float L, L1, alpha1, alpha2;
    L1 = sqrt(sq(x) + sq(y));
    gama = atan(x / y) / PI * 180;
    L = sqrt(sq(L1 - dim.CX_LENGHT) + sq(-z));
    beta = - (acos((sq(dim.TB_LENGHT) + sq(dim.FM_LENGHT) - sq(L)) / (2 * dim.TB_LENGHT * dim.FM_LENGHT)) / PI * 180) + 180;
    alpha1 = acos(-z / L) / PI * 180;
    alpha2 = acos((sq(dim.FM_LENGHT) + sq(L) - sq(dim.TB_LENGHT)) / (2 * dim.FM_LENGHT * L)) / PI * 180;
    alpha = - (alpha1 + alpha2) + 90;
  }
}

void RmatrixX(float &Px, float &Py, float &Pz, float alfa, float Cx, float Cy, float Cz) {
  float TPx = Px;
  float TPy = Py;
  float TPz = Pz;
  Py = cos(toRad(alfa)) * (TPy - Cy) + sin(toRad(alfa)) * (TPz - Cz) + Cy;
  Pz = cos(toRad(alfa)) * (TPz - Cz) - sin(toRad(alfa)) * (TPy - Cy) + Cz;
}

void RmatrixY(float &Px, float &Py, float &Pz, float beta, float Cx, float Cy, float Cz) {
  float TPx = Px;
  float TPy = Py;
  float TPz = Pz;
  Px = cos(toRad(beta)) * (TPx - Cx) - sin(toRad(beta)) * (TPz - Cz) + Cx;
  Pz = sin(toRad(beta)) * (TPx - Cx) + cos(toRad(beta)) * (TPz - Cz) + Cz;
}

void RmatrixZ(float &Px, float &Py, float &Pz, float gamma, float Cx, float Cy, float Cz) {
  float TPx = Px;
  float TPy = Py;
  float TPz = Pz;
  Px = cos(toRad(gamma)) * (TPx - Cx) + sin(toRad(gamma)) * (TPy - Cy) + Cx;
  Py = cos(toRad(gamma)) * (TPy - Cy) - sin(toRad(gamma)) * (TPx - Cx) + Cy;
}

void Rmatrix(float &x_, float &y_, float &z_, float alfa, float beta, float gama, float centerX_, float centerY_, float centerZ_) {
  RmatrixX(x_, y_, z_, alfa, centerX_, centerY_, centerZ_);
  RmatrixY(x_, y_, z_, beta, centerX_, centerY_, centerZ_);
  RmatrixZ(x_, y_, z_, gama, centerX_, centerY_, centerZ_);
}

void Spline2D(float p[2][11], float ti, float &a, float &b) {
  float calc[11];
  float calc_[11];
  for (int i = 0; i < 11; i++) {
    calc[i] = p[0][i];
    calc_[i] = p[1][i];
  }
  for (int i = 0; i < 11; i++) {
    for (int j = 0; j < 10 - i; j++) {
      calc[j] = timeVal(ti, calc[j], calc[j + 1]);
      calc_[j] = timeVal(ti, calc_[j], calc_[j + 1]);
    }
  }
  a = calc[0];
  b = calc_[0];
}

void Spline3D(float p[3][4], float ti, float &a, float &b, float &c) {
  float calc[4];
  float calc_[4];
  float calc__[4];
  for (int i = 0; i < 4; i++) {
    calc[i] = p[0][i];
    calc_[i] = p[1][i];
    calc__[i] = p[2][i];
  }
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 3 - i; j++) {
      calc[j] = timeVal(ti, calc[j], calc[j + 1]);
      calc_[j] = timeVal(ti, calc_[j], calc_[j + 1]);
      calc__[j] = timeVal(ti, calc__[j], calc__[j + 1]);
    }
  }
  a = calc[0];
  b = calc_[0];
  c = calc__[0];
}

float timeVal(float t, float higher, float lower) { //0->1 = higher->lower
  return t * (higher - lower) + lower;
}

float toRad(float deg) {
  return deg * (PI / 180.0);
}

float toDeg(float rad) {
  return rad * (180.0 / PI);
}

float fmap(float x, float in_min, float in_max, float out_min, float out_max) {
  return (float)(x - in_min) * (out_max - out_min) / (float)(in_max - in_min) + out_min;
}

bool compareMatrix(float matrix[3][8], const float matrix1[3][8]) {
  bool isEqual = true;
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 3; j++) {
      if (matrix[j][i] != matrix1[j][i]) {
        isEqual = false;
      }
    }
  }
  return isEqual;
}

float limit(float val, float downlimit, float uplimit) {
  if (val > uplimit) {
    val = uplimit;
  }
  else if (val < downlimit) {
    val = downlimit;
  }
  return val;
}

lp_filter::lp_filter(double rc, double start_val){
  rc_constant = rc;
  prev_val = start_val;
}

lp_filter::lp_filter(double rc){
  rc_constant = rc;
}

double lp_filter::get_val(){
  double interv = (micros() - prev_time)/1000000.0;
  double a = interv / (rc_constant + interv);
  double val = ((1.0 - a) * prev_val) + (a * setpoint);
  prev_val = val;
  prev_time = micros();
  return val;
}

void lp_filter::set_input(double val){
  setpoint = val;
}

void lp_filter::set_constant(double constant){
  rc_constant = constant;
}

