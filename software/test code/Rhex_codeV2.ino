#include "Rhex_Hexapod.h"
#include "Rhex_serial.h"

Hexapod Rhex;

int remote[5] = {};
bool repositioned = false;
byte prevfunction;

void setup() {
  Rhex.init();
  decodeSerialComm(remote);
  Rhex.standUp();
}

void loop() {
  decodeSerialComm(remote);
  switch (remote[4]-100) {
    case 0:
      if(prevfunction != 0){
        Rhex.stand();
      }
      prevfunction=0;
      if (remote[2] > -200 and remote[2]<200 and remote[3]> -200 and remote[3] < 200) {
        Rhex.stand();
      }
      else {
        Rhex.step(remote);
      }
    break;
    case 1:
      /*if(prevfunction != 1){
        Rhex.stand();
      }*/
      prevfunction=1;
      Rhex.move(remote);
    break;
    case 2:
      /*if(prevfunction != 2){
        Rhex.stand();
      }*/
      prevfunction=2;
      Rhex.moveleg(remote);
    break;
  }
}
