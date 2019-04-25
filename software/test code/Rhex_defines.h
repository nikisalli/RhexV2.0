#ifndef RHEX_DEFINES_H
#define RHEX_DEFINES_H

#include "Arduino.h"
#include "Rhex_structs.h"
//
//                        /
//                      /    ______
//                    /     |      |                   |-|-----|-|
//                  FM     /| +--+ | ^              O               O
//                 /     /  |______| |              | \           / |
//               /     /    / |  |   |        LEG 1 |  \   _N_   /  | LEG 6
//             /     /    /   |  |   |              |   \ __X__ /   |
//      <--CX-->   /    /     |  |   |          O\       X     X       /O
//       ___ ____/_   /       |  |   |    LEG 2 |   \    )     (    /   |
//------|   |      |/         |  |  TB          |      \X       X/      | LEG 5
//      | + | +--+ |          |  |   |          |        )     (        |
//------|___|______|          |  |   |              O----X__ __X----O
//                            |__|   |              |       X       |
//                            |  |   |        LEG 3 |      .^.      | LEG 4
//                            |  |   |              |     |   |     |
//                            |__|   v                     \ /
//

#define LEFT true //just because looks better in the code
#define RIGHT false //same ^

//==========================================//
//===========BODY & LEGS DIMENSIONS=========//
//==========================================//

// Dimension of each segment of each leg in centimeters.
//                                L1C   L1F   L1T  L2C   L2F   L2T  L3C   L3F   L3T  L4C   L4F   L4T  L5C   L5F   L5T  L6C   L6F   L6T
const float LEG_DIMENSIONS[18] = {5.5, 10.0, 13.5, 5.5, 10.0, 13.5, 5.5, 10.0, 13.5, 5.5, 10.0, 13.5, 5.5, 10.0, 13.5, 5.5, 10.0, 13.5};

// Offset of each servo in degrees.
//                             L1C L1F L1T L2C L2F L2T L3C L3F L3T L4C L4F L4T L5C L5F L5T L6C L6F L6T
const int SERVO_OFFSETS[18] = {40,  0, -8,  3,  0,  0, -45,  0,  0, 40,  0,  0,  0,  0,  8, -40,  0,  0};

// Side of each leg in  respect to the body.
//                       LEG1  LEG2  LEG3  LEG4  LEG5  LEG6
const bool LEG_SIDE[6] = {LEFT, LEFT, LEFT, RIGHT, RIGHT, RIGHT};

#define BODY_LENGHT   15.6
#define BODY_WIDTH_MAX    15.6
#define BODY_WIDTH_MIN   10.3

//   /\      LEG1------LEG6         -
//  /||\     /           \          |
//   ||     /     (+)     \         | LEG6 X OFFSET
//   ||    /               \        |
//        /                 \       |
//      LEG2  (-)  O  (+)   LEG5    -
//        \    X,Y CENTER   /       |
//         \      0,0      /        | LEG4 X OFFSET
//          \     (-)     /         |
//           \           /          |
//  X        LEG3------LEG4         -
//  ^    |-----|----|----|-----|
//  |     LEG2  LEG3 LEG4  LEG5   Y OFFSET
//  | Z         LEG1 LEG6
//  +------> Y

// Offset of each leg attach point to the body in respect to the center O of the body in cm.
//                                  L1X   L1Y   L2X   L2Y   L3X   L3Y   L4X   L4Y  L5X   L5Y   L6X   L6Y
const float LEG_START_OFFSET[12] = {(BODY_LENGHT/2), -(BODY_WIDTH_MIN/2),   0, -(BODY_WIDTH_MAX/2), -(BODY_LENGHT/2), -(BODY_WIDTH_MIN/2), -(BODY_LENGHT/2),  (BODY_WIDTH_MIN/2),   0,  (BODY_WIDTH_MAX/2), (BODY_LENGHT/2),  (BODY_WIDTH_MIN/2)};

//==========================================//
//===========WALK & STAND SETTINGS==========//
//==========================================//

// Step settings.
#define MAX_STEP_LENGHT 10 //maximum arc lenght covered by two points of a natural spline while walking
#define MAX_STEP_HEIGHT 5 //natural spline max point height from ground
#define MINIMUM_STEP_TIME 0.6 //minimum time for executing a step
#define WALK_FRAMES 50
#define START_Z 0
#define STAND_Z -6
const int RANDOM_SEQ[10][6] = { //i made this by randomly pushing keys on my keyboard so it's random enough
  {1,3,5,2,0,4},
  {3,5,1,0,4,2},
  {5,1,3,0,2,4},
  {3,5,1,4,2,0},
  {3,1,5,4,0,2},
  {5,3,1,0,4,2},
  {1,3,5,2,0,4},
  {3,5,1,2,4,0},
  {1,5,3,4,2,0},
  {5,1,3,2,0,4},
};
// Natural position. Used while standing or doing nothing and to calculate everything with it.
const float STAND_POS[3][6] = {
// L1   L2   L3   L4   L5   L6
  { 15,  0, -15, -15,  0, 15},  //X
  { -15, -20, -15, 15, 20, 15}, //Y
  { STAND_Z, STAND_Z, STAND_Z, STAND_Z, STAND_Z, STAND_Z},    //Z
};

//      C       BEZIER CURVE NATURAL SPLINE     D
//        O-----------------------------------O
//         \         XXXXXXXXXXXXXXX         /
//          \      XX               XX      /
//           \    X                   X    /
//            \   X                   X   /
//             \   X                 X   /
//              \   XX             XX   /
//               O----^^^^^^@^^^^^^----O
//             B     A     0,0     F     E


//==========================================//
//===========ELECTRONICS SETTINGS===========//
//==========================================//
// Servo Pins
const int SERVO_PINS[18] = {PD8, PD11, PD9, PE15, PE12, PE13, PE10, PE11, PE14, PD10, PD12, PD13, PC9, PC6, PC8, PE8, PE9, PB1};

#define SENSORS_SDA PB11 //SENSORS SDA
#define SENSORS_SCL PB10 //SENSORS SCL

#define L3G4200D_ADDRESS 0x69
#define CTRL_REG1 0x20
#define CTRL_REG2 0x21
#define CTRL_REG3 0x22
#define CTRL_REG4 0x23
#define CTRL_REG5 0x24

#define HMC5883L_ADDRESS 0x1E

#define ADXL345_ADDRESS 0x53

#define COMMUNICATION_MODULE_BAUD_RATE 9600 //HC_12 baud rate (9600 baud is slow but at this speed the module has maximum power and sensibility while trasmitting or receiving data)
#define SIDE_COMPUTER_COMMUNICATION_BAUD_RATE 115200 //Raspberry pi 3b+ baud rate (this can be pretty fast)
#define COMMUNICATION_MODULE_TX PA2 //Serial2 TX
#define COMMUNICATION_MODULE_RX PA3 //Serial2 RX
#define SIDE_COMPUTER_TX PA9 //Serial1 TX
#define SIDE_COMPUTER_RX PA10 //Serial1 RX

#endif //RHEX_DEFINES_H
