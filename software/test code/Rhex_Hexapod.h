#ifndef RHEX_HEXAPOD_H
#define RHEX_HEXAPOD_H

class Hexapod{
  public:
    Hexapod();
    void init();
    void stand();
    void step(int dir[]);
    void standUp();
    void move(int dir1[]);
    void moveleg(int dir[]);
};


void stepCalc(int walk_pattern, float t, int Cx, int Cy, float height, float actualpos[][6], float arc);

#endif //RHEX_HEXAPOD_H
