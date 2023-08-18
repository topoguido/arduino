#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>

TinyGPSPlus gps;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

static const int RXPin = 14, TXPin = 13;
SoftwareSerial ss(RXPin, TXPin);// GPIO14 and GPIO13 (PIN D5 and D7)

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
  pinMode(12, OUTPUT); 
  digitalWrite(12, LOW);//LED and D6: Valid GPS Data

  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
  WiFi.forceSleepBegin();
  yield();

   // Serial:
  Serial.begin(115200);
  // SoftwareSerial:
  ss.begin(9600);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  Serial.println("Inicio");
  delay(2000);
  display.clearDisplay();

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

    //myPrint(col, row, String("Dentro del while"), false, true);
    Serial.println(ss.read());
    if (gps.encode(ss.read()))
    { 
      myPrint(col, row, String("GPS activo"), false, true);
      delay(500);
      if (gps.location.isValid())
      {
        //myPrint(col, row, String("Ubicación válida"), false, true);
        //delay(500);

        latitude = gps.location.lat();
        lat_str = String(latitude , 6);
        longitude = gps.location.lng();
        lng_str = String(longitude , 6);
        alt_str = String(gps.altitude.meters());
        sats = gps.satellites.value();
        sats_str = String(sats);

        digitalWrite(12, HIGH); 

        myPrint(col, row, lat_str, false, true);
        myPrint(col, row, lng_str, true, true);
        myPrint(col, row, sats_str, true, true);
        myPrint(col, row, alt_str, true, true);

      }else{
        myPrint(col, row, String("Buscando satélites"), false, true);
		    digitalWrite(12, LOW);        
      }
    } else{
      myPrint(col, row, String("Sin GPS"), false, true);
    }

    delay(500);
    display.clearDisplay();
  }
}

void myPrint(uint8_t col, uint8_t row, String message, bool newLine, bool serial){
  if(!message.isEmpty()){
    
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
