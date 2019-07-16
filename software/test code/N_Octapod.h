#ifndef N_OCTAPOD_H
#define N_OCTAPOD_H

#include"N_defines.h"
#include"N_math.h"

class Octapod{
  public:
    float Cx;
    float Cy;
    float arc;
    float walk_circ;
    float walk_ang;
    float walk_ray;
    
    Octapod();
    void init();
    void stand();
    void force_stand();
    void step(int walk_pattern, float height, float time);
    void standUp();
    void move(float alpha, float beta, float gamma, float x_move, float y_move, float z_move);
    void moveleg();
    void sit();
};

#endif
