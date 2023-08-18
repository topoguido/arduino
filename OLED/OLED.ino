#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "graph.h"

#define ANCHO 128
#define ALTO 64

#define OLED_RESET -1
Adafruit_SSD1306 oled(ANCHO, ALTO, &Wire, OLED_RESET);

uint8_t x = 0;
uint8_t y = 0;
uint8_t count = 0;

void setup(){
  Wire.begin();
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  Serial.begin(115200);

  oled.clearDisplay();
  oled.drawBitmap(x, y, logo, 128, 64, WHITE); 
  oled.display();
  delay(4000);

  //oled.setRotation(1);
}

void loop(){
  if(count > 3)
  {
    count = 0;
  }
  oled.clearDisplay();
  oled.drawBitmap(x+30, y, sat[count], 64, 64, WHITE); 
  oled.display();
  delay(500);
  count++;
}
