#include "Adafruit_GFX.h"
#include "gfxfont.h"
#include "Adafruit_ILI9341_8bit_STM.h"

Adafruit_ILI9341_8bit_STM tft = Adafruit_ILI9341_8bit_STM();

int pots[6] = {};
int ppots[6] = {};
int pins[5] = {PA4, PA5, PA6, PA7, PA3};
char letters[5] = {'a', 'b', 'c', 'd', 'e'};
int time;
float batt_volt;
String readString;
unsigned long time1;
unsigned long mil;

void setup() {
  Serial1.begin(9600);
  for (int i = 0; i < 4; i++) {
    pinMode(pins[i], OUTPUT);
    digitalWrite(pins[i], LOW);
  }
  pinMode(PA8,INPUT_PULLUP);
  pinMode(PB15,INPUT_PULLUP);
  tft.begin();
  tft.fillScreen(0x0000);
  tft.setRotation(1);
}

void loop() {
  writeSerial();
  delay(30);
  readPot();

  if (pots[0] / 42 != ppots[0] / 42 or pots[1] / 42 != ppots[1] / 42 or pots[2] / 42 != ppots[2] / 42 or pots[3] / 42 != ppots[3] / 42) {
    tft.fillRect(0, 0, 20, 130, 0x0000);
    tft.fillRect(20, 0, 20, 130, 0x0000);
    tft.fillRect(275, 0, 20, 130, 0x0000);
    tft.fillRect(295, 0, 20, 130, 0x0000);

    tft.fillRect(10, 10, 3, 120, 0x528A);
    tft.fillCircle(11, 15 + (pots[0] / 42), 4, 0x07E8);

    tft.fillRect(30, 10, 3, 120, 0x528A);
    tft.fillCircle(31, 15 + (pots[2] / 42), 4, 0x07E8);

    tft.fillRect(280, 10, 3, 120, 0x528A);
    tft.fillCircle(281, 15 + (pots[3] / 42), 4, 0x07E8);

    tft.fillRect(300, 10, 3, 120, 0x528A);
    tft.fillCircle(301, 15 + (pots[1] / 42), 4, 0x07E8);
  }

  if ((millis() - time) > 1000) {
    tft.fillRect(50, 10, 120, 30, 0x0000);
    tft.setCursor(50, 10);
    tft.setTextColor(0x07E8);
    tft.setTextSize(2);
    tft.print(12.0);
    tft.print("V");
    for (int i = 0; i < 70; i++) {
      tft.drawLine(50 + i, 36, 50 + i, 36 - (i / 6), 0x07E8);
    }
 
    tft.setCursor(170, 10);
    tft.fillRect(145, 10, 120, 20, 0x0000);
    tft.print((int) (millis() / 1000) % 60);
    tft.print(":");
    tft.print((int) (millis() / 60000) % 60);
    tft.print(":");
    tft.print((int) (millis() / 3600000) % 24);
    time = millis();
    

    tft.fillRect(50, 50, 80, 30, 0x0000);
    tft.setCursor(50, 50);
    tft.print(batt_volt);
    tft.print("V");
    tft.print("  ");
    for (int i = 0; i < fmap(batt_volt, 3.5, 4.2, 1, 70); i++) {
      tft.drawLine(50 + i, 76, 50 + i, 76 - (i / 6), 0x07E8);
    }
    if(batt_volt<3.6){
      tft.fillTriangle(120,64,140,64,130,50,0xFE60);
      tft.drawTriangle(120,64,140,64,130,50,0x0000);
      tft.setTextSize(1);
      tft.setCursor(128,54);
      tft.setTextColor(0x0000);
      tft.print("!");
      tft.setTextColor(0xF800);
      tft.print("   low voltage!");
      tft.setTextColor(0x07E8);
    }
    else{
      tft.setTextSize(1);
      tft.fillTriangle(120,64,140,64,130,50,0x0000);
      tft.setTextSize(1);
      tft.setCursor(128,54);
      tft.setTextColor(0x0000);
      tft.print("   low voltage!");
      tft.setTextColor(0x07E8);
    }
  }

  tft.setCursor(200, 220);
  tft.print("by niklas sallali");

  tft.fillRoundRect(52, 122, 206, 86, 6, 0x0000);
  tft.drawRoundRect(50, 120, 210, 90, 6, 0x528A);
  if(digitalRead(PB15)){
    tft.fillRect(165,30,12,12,0x07E8);
    tft.setCursor(180, 33);
    tft.print("bt1");
  }
  else{
    tft.fillRect(166,31,10,10,0x0000);
    tft.setCursor(180, 33);
    tft.print("bt1");
  }
  if(digitalRead(PA8)){
    tft.fillRect(200,30,12,12,0x07E8);
    tft.setCursor(215, 33);
    tft.print("bt2");
  }
  else{
    tft.fillRect(201,31,10,10,0x0000);
    tft.setCursor(215, 33);
    tft.print("bt2");
  }
  if(!digitalRead(PA8) and !digitalRead(PB15) and (millis()-time1)>300){
    time1 = millis();
    pots[5]++;
    if(pots[5]>4){
      pots[5]=0;
    }
  }
  tft.setCursor(60, 130);
  switch(pots[5]){
    case 0:
      tft.print("walking mode");
    break;
    case 1:
      tft.print("6 axis control mode");
    break;
    case 2:
      tft.print("leg control mode");
    break;
    case 3:
      tft.print("self leveling mode");
    break;
    case 4:
      tft.print("object tracking mode");
    break;
  }
  tft.setCursor(60, 140);
  tft.print("data sent:");
  tft.setCursor(60, 150);
  for (int i = 0; i < 4; i++) {
    tft.print(map(pots[i], 0, 4096, 101, 999));
    tft.print(' ');   
  }
  tft.print(pots[5]+100);
  tft.setCursor(60, 160);
  tft.print("active time millis:");
  tft.print(millis());
  tft.setCursor(60, 170);
  tft.print("data packs per second:");
  tft.print(1/((millis()-mil)/1000.0));
  mil = millis();
  tft.setCursor(60,180);
  tft.print("real time battery:");
  tft.print(batt_volt);
}

void readPot() {
  for (int i = 0; i < 4; i++) {
    pinMode(pins[i], INPUT);
    ppots[i] = pots[i];
    pots[i] = analogRead(pins[i]);
    pinMode(pins[i], OUTPUT);
  }
  pinMode(pins[4], INPUT);
  for (int i = 0; i < 30; i++){
    pots[4] = (analogRead(pins[4])+ppots[4])/2;
    ppots[4] = pots[4];
  }
  pinMode(pins[4], OUTPUT);
  ppots[4] = pots[4];
  batt_volt = (float)pots[4] / 590;
}

void writeSerial() {
  for (int i = 0; i < 4; i++) {
    Serial1.print(map(pots[i], 0, 4096, 101, 999));
    Serial1.print(letters[i]);
    Serial1.print(',');   
  }
  Serial1.print(pots[5]+100);
  Serial1.print("e,");
}


float fmap(float x, float in_min, float in_max, float out_min, float out_max) {
  return (float)(x - in_min) * (out_max - out_min) / (float)(in_max - in_min) + out_min;
}
