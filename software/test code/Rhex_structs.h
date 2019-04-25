#ifndef RHEX_STRUCTS_H
#define RHEX_STRUCTS_H

struct Dleg{
  float CX_LENGHT,FM_LENGHT,TB_LENGHT,START_X_OFFSET,START_Y_OFFSET;
  bool SIDE;
  Dleg(){}
  Dleg(float CX_LENGHT, float FM_LENGHT, float TB_LENGHT, float START_X_OFFSET, float START_Y_OFFSET, bool SIDE){
    this->CX_LENGHT = CX_LENGHT;
    this->FM_LENGHT = FM_LENGHT;
    this->TB_LENGHT = TB_LENGHT;
    this->START_X_OFFSET = START_X_OFFSET;
    this->START_Y_OFFSET = START_Y_OFFSET;
    this->SIDE = SIDE;
  }
};
  
#endif
