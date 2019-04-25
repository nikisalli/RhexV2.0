#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341_8bit_STM.h"
#include "image.h"

Adafruit_ILI9341_8bit_STM tft = Adafruit_ILI9341_8bit_STM();

int lol;
void setup() {
  tft.begin();
  tft.setTextColor(ILI9341_GREEN);
  tft.setTextSize(2);
  tft.fillScreen(ILI9341_BLACK);
  tft.setRotation(1);
}

void loop() {
  int b=millis();
  for (int i = 0; i < 160; i++) {
    for (int j = 0; j < 320; j++) {
      tft.drawPixel(j, i+36, a[(320 * i) + j]);
    }
  }
  tft.setTextColor(ILI9341_BLACK);
  tft.setCursor(0, 0);
  tft.print(lol);
  lol = 1/((millis()-b)/1000.0);
  tft.setTextColor(ILI9341_GREEN);
  tft.setCursor(0, 0);
  tft.print(lol);
  tft.print("fps");
}
/*int pots[4] = {};
  int pins[4] = {PA4,PA5,PA6,PA7};

  void setup() {
  for(int i=0;i<4;i++){
    pinMode(pins[i],OUTPUT);
    digitalWrite(pins[i],LOW);
  }
  tft.begin();
  tft.setRotation(1);
  tft.setTextColor(ILI9341_GREEN);
  tft.setTextSize(2);
  }

  void loop() {
  readPot();
  tft.setCursor(0, 0);
  delay(20);
  tft.fillScreen(ILI9341_BLACK);
  for(int i=0;i<4;i++){
    tft.println(pots[i]);
  }
  }

  void readPot(){
  for(int i=0;i<4;i++){
    pinMode(pins[i],INPUT);
    pots[i] = analogRead(pins[i]);
    pinMode(pins[i],OUTPUT);
    delay(2);
  }
  }*/

