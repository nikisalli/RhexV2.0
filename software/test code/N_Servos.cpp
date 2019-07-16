#include "N_Servos.h"
#include "N_defines.h"
#include "N_Octapod.h"
#include "N_math.h"
#include "N_structs.h"

extern struct Dleg Dlegs[8];

bool active_servo[26] = {};
int variable_angles[26] = {};
bool written = false;
bool writing = false;

TaskHandle_t Task2;

void Task2code( void * parameter) {
  for (;;) {
    if(written){
      if (!writing){
        for (int i = 0; i < 24; i++) {
          if (active_servo[i]){
            servoMove((variable_angles[i]+SERVO_OFFSETS[i]),i+1);
          }
        }
      }
    }
    else{
      delay(10);
    }
  }
}

void servoInit(){
  Serial2.begin(115200);
  Serial2.setRxBufferSize(1024);
  
  pinMode(18, OUTPUT);
  pinMode(19, OUTPUT);

  for(int i=0;i<26;i++){
    active_servo[i] = true;
  }
  
  xTaskCreatePinnedToCore(Task2code, "Task2", 10000, NULL, 1, &Task2, 1);
}

bool bodyToServos(float pos[][8]){
  float lol[24];
  for(int i=0;i<4;i++){
     trigz(lol[3*i],lol[3*i+1],lol[3*i+2],-(pos[0][i]-LEG_START_OFFSET[i*2]),-(pos[1][i]-LEG_START_OFFSET[i*2+1]),pos[2][i],Dlegs[i]);
  }
  for(int i=4;i<8;i++){
     trigz(lol[3*i],lol[3*i+1],lol[3*i+2],-(pos[0][i]-LEG_START_OFFSET[i*2]),pos[1][i]-LEG_START_OFFSET[i*2+1],pos[2][i],Dlegs[i]);
  }
  servoWrite(lol);
}

bool servoWrite(float angles[]){
  for (int i = 0; i < 24; i++) {
    written = true;
    variable_angles[i] = angles[i];
  }
}

void bodyDetach(){
  writing = true;
  for(int i=0;i<26;i++){
    if(active_servo[i]){
      servoDetach(i);
      active_servo[i] = false;
    }
  }
  writing = false;
}

void bodyAttach(){
  writing = true;
  for(int i=0;i<26;i++){
    if(!active_servo[i]){
      servoAttach(i);
      active_servo[i] = true;
    }
  }
  writing = false;
}

void servoMove(int ang, byte id){
  digitalWrite(SERVO_PIN_TX_ENB,HIGH);
  digitalWrite(SERVO_PIN_RX_ENB,LOW);
  
  ang = limit(map(ang, -120, 120, 0, 1000),0,1000);
  
  Serial2.write(0x55);
  Serial2.write(0x55);
  Serial2.write(id);
  Serial2.write(0x07);
  Serial2.write(0x01);
  Serial2.write(ang & 0xFF);
  Serial2.write(ang >> 8);
  Serial2.write(0x00);
  Serial2.write(0x00);
  Serial2.write(~((byte)(id+0x08+(ang & 0xFF)+(ang >> 8))));
  
  //delayMicroseconds(1100);
  //Serial2.flush();
  digitalWrite(SERVO_PIN_TX_ENB,LOW);
  digitalWrite(SERVO_PIN_RX_ENB,HIGH);
}

void servoAttach(byte id){
  active_servo[id] = true;
  digitalWrite(SERVO_PIN_TX_ENB,HIGH);
  digitalWrite(SERVO_PIN_RX_ENB,LOW);
  
  Serial2.write(0x55);
  Serial2.write(0x55);
  Serial2.write(id);
  Serial2.write(0x04);
  Serial2.write(0x1F);
  Serial2.write(0x01);
  Serial2.write(~((byte)(id+0x24)));
  
  Serial2.flush();
  digitalWrite(SERVO_PIN_TX_ENB,LOW);
  digitalWrite(SERVO_PIN_RX_ENB,HIGH);
}

void servoDetach(byte id){
  active_servo[id] = false;
  
  digitalWrite(SERVO_PIN_TX_ENB,HIGH);
  digitalWrite(SERVO_PIN_RX_ENB,LOW);
  
  Serial2.write(0x55);
  Serial2.write(0x55);
  Serial2.write(id);
  Serial2.write(0x04);
  Serial2.write(0x1F);
  Serial2.write(0x00);
  Serial2.write(~((byte)(id+0x23)));

  Serial2.flush();
  digitalWrite(SERVO_PIN_TX_ENB,LOW);
  digitalWrite(SERVO_PIN_RX_ENB,HIGH);
}

int servoReadPos(byte id){
  digitalWrite(SERVO_PIN_TX_ENB,HIGH);
  digitalWrite(SERVO_PIN_RX_ENB,LOW);

  Serial2.write(0x55);
  Serial2.write(0x55);
  Serial2.write(id);
  Serial2.write(0x03);
  Serial2.write(0x1C);
  Serial2.write(~((byte)(id+0x1F)));

  Serial2.flush();
  digitalWrite(SERVO_PIN_TX_ENB,LOW);
  digitalWrite(SERVO_PIN_RX_ENB,HIGH);

  unsigned long time = micros();
  while(!Serial2.available()){
    if((micros()-time)>3000){
      return 4000;
    }
  }
  byte tentatives = 0;
  while(Serial2.read() != 0x1C){
    tentatives++;
    if(tentatives>10){
      return 4000;
    }
  }
  byte low = Serial2.read();
  byte high = Serial2.read();
  byte checksum = Serial2.read();
  byte checksum_ = (~((byte)(id+0x21+low+high)));
  if(checksum == checksum_){
    return fmap((low|(high<<8)),0,1000,0,180);
  }
  else{
    return 4000;
  }
}

