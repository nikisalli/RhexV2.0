#ifndef N_DEFINES_H
#define N_DEFINES_H

#include "Arduino.h"
#include "N_structs.h"
//
//                        /
//                      /    ______
//                    /     |      |                    |-|-----|-|
//                  FM     /| +--+ | ^               O               O
//                 /     /  |______| |               | \           / |
//               /     /    / |  |   |         LEG 1 |  \  (:::)  /  | LEG 8
//             /     /    /   |  |   |               |   \ __X__ /   |
//      <--CX-->   /    /     |  |   |          O\        X     X        /O
//       ___ ____/_   /       |  |   |    LEG 2 |   \     )     (     /   |
//------|   |      |/         |  |  TB          |      \X         X/      | LEG 7
//      | + | +--+ |          |  |   |          |O__     )       (     __O|
//------|___|______|          |  |   |           |   ''\X         X/''   |
//                            |__|   |           |        )     (        | 
//                            |  |   |    LEG 3  | O______X_____X______O |  LEG 6
//                            |  |   |             |        ]|[        |
//                            |__|   v             |       || ||       |
//                                          LEG 4  |       || ||       |  LEG 5
//                                                         |___|


#define LEFT true //just because it makes the code look better
#define RIGHT false //same ^

#define PI 3.141592653

//==========================================//
//===========BODY & LEGS DIMENSIONS=========//
//==========================================//

// Dimension of each segment of each leg in centimeters.
//                                  L1C  L1F   L1T   L2C  L2F   L2T   L3C  L3F   L3T   L4C  L4F   L4T    L5C  L5F   L5T   L6C  L6F   L6T   L7C  L7F   L7T   L8C  L8F   L8T
const float LEG_DIMENSIONS[24] = {  6.5, 13.0, 17.0, 6.5, 13.0, 17.0, 6.5, 13.0, 17.0, 6.5, 13.0, 17.0,  6.5, 13.0, 17.0, 6.5, 13.0, 17.0, 6.5, 13.0, 17.0, 6.5, 13.0, 17.0};

// Offset of each servo in degrees.
//                                  L1C  L1F   L1T   L2C  L2F   L2T   L3C  L3F   L3T   L4C  L4F   L4T    L5C  L5F   L5T   L6C  L6F   L6T   L7C  L7F   L7T   L8C  L8F   L8T
const int SERVO_OFFSETS[24]    = {-45.3,    0,    0,-9.1,    0,    0, 9.1,    0,    0,45.3,    0,    0,-45.3,    0,    0,-9.1,    0,    0, 9.1,    0,    0,45.3,    0,    0};

// Side of each leg in  respect to the body.
//                        LEG1  LEG2  LEG3  LEG4  LEG5   LEG6   LEG7   LEG8
const bool LEG_SIDE[8] = {LEFT, LEFT, LEFT, LEFT, RIGHT, RIGHT, RIGHT, RIGHT};

#define BODY_LENGHT_MAX  21.6
#define BODY_LENGHT_MIN  8.0
#define BODY_WIDTH_MAX   23.0
#define BODY_WIDTH_MIN   14.0

//   /\      LEG1------LEG8         -  -           
//  /||\     /           \          |  |           
//   ||     /             \         |  |           
//   ||    /               \        |  |           
//        /       (+)       \       |  | LEG1 LEG8   
//      LEG2                LEG7    -  | X OFFSET   - LEG2 LEG7
//       |                    |     |  |            | X OFFSET
//       |    (-)  O  (+)     |     -  -            - 
//       |     X,Y CENTER     |     |  |            | LEG3 LEG6
//      LEG3      0,0       LEG6    -  |            - X OFFSET
//        \       (-)       /       |  | LEG4 LEG5 
//         \               /        |  | X OFFSET  
//          \             /         |  |          
//           \           /          |  |           
//  X        LEG4------LEG5         -  -           
//  ^    |-----|----|----|-----|
//  |     LEG2  LEG1 LEG8  LEG7   Y OFFSET
//  | Z   LEG3  LEG4 LEG5  LEG6
//  +------> Y

// Offset of each leg attach point to the body in respect to the center O of the body in cm.
const float LEG_START_OFFSET[16] = {
               (BODY_LENGHT_MAX/2), -(BODY_WIDTH_MIN/2),    //L1X  L1Y
               (BODY_LENGHT_MIN/2), -(BODY_WIDTH_MAX/2),    //L2X  L2Y
              -(BODY_LENGHT_MIN/2), -(BODY_WIDTH_MAX/2),    //L3X  L3Y
              -(BODY_LENGHT_MAX/2), -(BODY_WIDTH_MIN/2),    //L4X  L4Y
              -(BODY_LENGHT_MAX/2),  (BODY_WIDTH_MIN/2),    //L5X  L5Y
              -(BODY_LENGHT_MIN/2),  (BODY_WIDTH_MAX/2),    //L6X  L6Y
               (BODY_LENGHT_MIN/2),  (BODY_WIDTH_MAX/2),    //L7X  L7Y
               (BODY_LENGHT_MAX/2),  (BODY_WIDTH_MIN/2)     //L8X  L8Y
               
};

//==========================================//
//===========WALK & STAND SETTINGS==========//
//==========================================//

// Step settings.
#define MAX_STEP_LENGHT 7.0 //maximum arc lenght covered by two points of a natural spline while walking
#define MAX_STEP_HEIGHT 5.0 //natural spline max point height from ground
#define MINIMUM_STEP_TIME 2.0 //minimum time for executing a step
#define WALK_FRAMES 30
#define START_Z -4
#define STAND_Z -12
#define BODY_X_SHIFT 0

const int RANDOM_SEQ[2][8] = { //i made this by randomly pushing keys on my keyboard so it's random enough
  {0,2,1,3, 4,6,5,7},
  {2,1,3,0, 7,5,6,4},
};

// Natural position. Used while standing or doing nothing and to calculate everything with it.
const float STAND_POS[3][8] = {
//        L1               L2               L3              L4               L5               L6              L7              L8
  { 27             , 10            ,-10            ,-27             ,-27             ,-10             , 10            , 27             },   //X
  {-17             ,-27            ,-27            ,-17             , 17             , 27             , 27            , 17             },   //Y
  {STAND_Z         ,        STAND_Z,        STAND_Z,         STAND_Z,         STAND_Z,         STAND_Z,        STAND_Z,         STAND_Z},   //Z
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

#define FAN1_PIN 21
#define FAN2_PIN 22
#define SERVO_PIN_TX_ENB 18
#define SERVO_PIN_RX_ENB 19
#define HC12_PIN_TX 2
#define HC12_PIN_RX 15
#define HC12_PIN_SET 13
#define BATT_VOLTAGE_READ_PIN 35
#define BATT_VOLTAGE_READ_FILTER_CONSTANT 2
#define BATT_CURR_READ_PIN 32
#define BATT_CURR_READ_FILTER_CONSTANT 1
#define HC12_BAUD_RATE 9600
#define DEF_FAN_SPEED 100
#define MAX_FAN_SPEED 255
#define BATT_MIN_VOLTAGE 7.0
#define BATT_MAX_VOLTAGE 8.5


#define OTA_WIFI_PWD "12344321"
#define OTA_WIFI_SSID "nik"
#define OTA_WIFI_HOST "esp32"

#endif
