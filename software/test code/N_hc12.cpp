#include "N_hc12.h"
#include "N_defines.h"
#include "N_sensors.h"

remote::remote(){
}

void remote::init(){
  Serial1.begin(HC12_BAUD_RATE, SERIAL_8N1, HC12_PIN_RX, HC12_PIN_TX);
  Serial1.setRxBufferSize(18);
  pinMode(13,OUTPUT);
  digitalWrite(13,HIGH);
}

bool remote::receive(){
  if(Serial1.available()>9){
    byte bytes[7];
    unsigned long val = micros();
    while(Serial1.read()!=0x55){
      if((micros()-val)>2000){
        return 0;
      }
    }
    Serial1.read();
    for(int i=0;i<7;i++){
      bytes[i] = Serial1.read();
    }
    if(((byte)(~(bytes[0]+bytes[1]+bytes[2]+bytes[3]+bytes[4]+bytes[5]))) == bytes[6]){
      angle1_ = bytes[0]*1.43426;
      angle2_ = bytes[2]*1.43426;
      speed1_ = bytes[1];
      speed2_ = bytes[3];
      active_ = bytes[4];
      state_ = bytes[5];
      return 1;
    }
  }
  else{
    return 0;
  }
}

void remote::write(){
  //Serial1.println((String)read_battery_voltage() + "V " + (String)read_battery_current() + "A");

  /*int vol = (int)(read_battery_voltage()*100);
  int cur = (int)(read_battery_current()*100);
  byte vol_up = vol >> 8;
  byte vol_lo = vol & 0xFF;
  byte cur_up = cur >> 8;
  byte cur_lo = cur & 0xFF;
  Serial1.write(0x55); //U
  Serial1.write(vol_up);
  Serial1.write(vol_lo);
  Serial1.write(cur_up);
  Serial1.write(cur_lo);
  Serial1.write(~((byte)(vol_up+vol_lo+cur_up+cur_lo)));
  Serial1.write(0x56); //V*/

  Serial1.println('#' + String(((int)(read_battery_voltage()*100))/100.0) + '+' + String(((int)(read_battery_current()*100))/100.0) + '+' + '*');
}
