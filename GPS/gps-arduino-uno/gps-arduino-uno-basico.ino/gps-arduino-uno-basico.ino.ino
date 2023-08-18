#define __DEBUG__
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <TinyGPSPlus.h>
#include <SPI.h>

TinyGPSPlus gps;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET -1

static const int RXPin = 14, TXPin = 12;
SoftwareSerial ss(RXPin, TXPin);// WEMOS: GPIO14 and GPIO12 (PIN D5 and D6)

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

uint8_t col = 0;
uint8_t row = 0;
uint8_t TEXT_SIZE = 1;

String lat_str , lng_str, alt_str, sats_str;

TinyGPSCustom pdop(gps, "GPGSA", 15); // $GPGSA sentence, 15th element
TinyGPSCustom hdop(gps, "GPGSA", 16); // $GPGSA sentence, 16th element
TinyGPSCustom vdop(gps, "GPGSA", 17); // $GPGSA sentence, 17th element
//=======================================================================
//                    Setup
//=======================================================================
void setup() {
   // Serial:
  Serial.begin(115200);
    // SoftwareSerial:
  ss.begin(9600);
  Wire.begin();
  
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
#ifdef __DEBUG__
    Serial.println("No se encuentra la pantalla OLED");
#endif
    while (true);
  }
  display.display();
  delay(2000);
  display.clearDisplay();
  display.setTextSize(TEXT_SIZE);
  display.setTextColor(WHITE);
  display.setCursor(col, row);

  Serial.println("Inicio");
  display.println("Inicio");
  display.display();
   
}
 
//=======================================================================
//                    Main Program Loop

void loop() {
 
  // GPS:
  if (gps.altitude.isUpdated() || gps.satellites.isUpdated() ||
    pdop.isUpdated() || hdop.isUpdated() || vdop.isUpdated())
  { 
      if(TEXT_SIZE == 1)
      {
        lat_str = "Lat:  " + String(gps.location.lat(), 6);
        lng_str = "Long: " + String(gps.location.lng(), 6);
        alt_str =  "   Alt:" + String(gps.altitude.meters());
        sats_str = "Sat:" + String(gps.satellites.value());
        myPrint(col, row, lat_str, false, true);
        myPrint(col, row, lng_str, true, true);
        myPrint(col, row, sats_str + alt_str, true, true);
      }
      if(TEXT_SIZE == 2)
      {
        lat_str = String(gps.location.lat(), 6);
        lng_str = String(gps.location.lng(), 6);
        myPrint(col, row, lat_str, false, true);
        myPrint(col, row, lng_str, true, true);
      }
  }
  display.display();
  while (ss.available() > 0)
  {
    gps.encode(ss.read());
  }
}

void myPrint(uint8_t col, uint8_t row, String message, bool newLine, bool serial)
{
  if(message.length() > 0){
    
    if(!newLine){
      display.clearDisplay();
      display.setCursor(col, row);  
    } else{
      col = 0;
      row++;
    }
    
    display.println(message);
    if(serial){
      Serial.println(message);
    }
    if(!newLine){
      row = row + message.length();
    } 
  }
}