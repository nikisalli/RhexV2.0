#include "Rhex_serial.h"
#include "Rhex_defines.h"
#include "Rhex_math.h"

String readString;

HardwareSerial Serial2(COMMUNICATION_MODULE_RX, COMMUNICATION_MODULE_TX);
HardwareSerial Serial1(SIDE_COMPUTER_RX, SIDE_COMPUTER_TX);

void initSerialComm() {
  Serial2.begin(COMMUNICATION_MODULE_BAUD_RATE);
}

void initSerialComp() {
  Serial1.begin(SIDE_COMPUTER_COMMUNICATION_BAUD_RATE);
}

void decodeSerialComm(int comm[5]) {
  //expect single strings like 700a, or 1500c, or 2000d,
  //or like 30c, or 90a, or 180d,
  //or combined like 30c,180b,70a,120d,
  while (!Serial2.available()){
    //wait for data to arrive
  }
  while (Serial2.available()) {
    char c = Serial2.read();  //gets one byte from serial buffer
    if (c == ',') {
      if (readString.length() > 1) {
        int n = readString.toInt();  //convert readString into a number
        if (readString.indexOf('a') > 0) comm[0] = limit(map(n,101,999,500,-500),-500,500);
        if (readString.indexOf('b') > 0) comm[1] = limit(map(n,101,999,500,-500),-500,500);
        if (readString.indexOf('c') > 0) comm[2] = limit(map(n,101,999,500,-500),-500,500);
        if (readString.indexOf('d') > 0) comm[3] = limit(map(n,101,999,500,-500),-500,500);
        if (readString.indexOf('e') > 0) comm[4] = limit(n,100,104);
        readString = ""; //clears variable for new input
      }
    }
    else {
      readString += c; //makes the string readString
    }
  }
}

