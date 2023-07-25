//#include <TinyGPS++.h>
#include <TinyGPSPlus.h>

#define __DEBUG__

//#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>
#include <Wire.h>

TinyGPSPlus gps;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET -1
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

static const int RXPin = 4, TXPin = 3;
SoftwareSerial ss(RXPin, TXPin);// WEMOS: GPIO14 and GPIO13 (PIN D5 and D7)

uint8_t col = 0;
uint8_t row = 0;

float latitude , longitude;
uint32_t sats;
String lat_str , lng_str, alt_str, sats_str;

//=======================================================================
//                    Setup
//=======================================================================
void setup() {

    //LEDs:
  //pinMode(12, OUTPUT); 
  //digitalWrite(12, LOW);//LED and D6: Valid GPS Data
 
  
   // Serial:
  Serial.begin(115200);
    // SoftwareSerial:
  ss.begin(9600);
  
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
#ifdef __DEBUG__
    Serial.println("No se encuentra la pantalla OLED");
#endif
    while (true);
  }
  Serial.println("Inicio");
  delay(2000);
  display.clearDisplay();
  //cleanMem();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(col, row);
   
}
 
//=======================================================================
//                    Main Program Loop

void loop() {
  
  // GPS:
   while (ss.available() > 0)
  {

    //Serial.println(ss.read());
    if (gps.encode(ss.read()))
    { 
      myPrint(col, row, String("GPS activo"), false, true);
      //delay(500);
      if (gps.location.isValid())
      {
        //myPrint(col, row, String("Ubicación válida"), false, true);
        //delay(500);

        latitude = gps.location.lat();
        lat_str = "Lat:" + String(latitude , 6);
        longitude = gps.location.lng();
        lng_str = "Long: " + String(longitude , 6);
        alt_str = String(gps.altitude.meters());
        sats = gps.satellites.value();
        sats_str = "Sats: " + String(sats);

       // digitalWrite(12, HIGH); 

        myPrint(col, row, lat_str, false, true);
        myPrint(col, row, lng_str, true, true);
        myPrint(col, row, sats_str, true, true);
        myPrint(col, row, alt_str, true, true);

      }else{
        myPrint(col, row, String("Buscando satélites"), false, true);
		   // digitalWrite(12, LOW);        
      }
    } else{
      myPrint(col, row, String("Sin GPS"), false, true);
    }

    //delay(500);
    display.clearDisplay();
  }
}

void myPrint(uint8_t col, uint8_t row, String message, bool newLine, bool serial){
  //Serial.println("message length: " + String(message.length()));
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

    display.display();

  }

}

void cleanMem(){
     for(int i = 0; i < 1024; i++){
        Wire.beginTransmission(0x3C);
        Wire.write(0x40);
        Wire.write(0x00);
        Wire.endTransmission();
        }
  }
