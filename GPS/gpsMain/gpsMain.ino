#include "LiquidCrystal_I2C.h"
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
TinyGPSPlus gps;
SoftwareSerial ss(14, 12);// GPI14 and GPI12 (PIN D5 and D6)
float latitude , longitude;
uint32_t sats;
String lat_str , lng_str, sats_str;

int lcdColumns = 16;
int lcdRows = 2;

LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

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

  lcd.begin(5,4);
  lcd.init();
  lcd.backlight();
  Serial.println("Inicio");
}
 
//=======================================================================
//                    Main Program Loop

void loop() {

  // GPS:
  while (ss.available() > 0)
  {
    lcd.setCursor(0,0);
    Serial.println("Dentro del while");
    lcd.print("Dentro del while");
    if (gps.encode(ss.read()))
    { 
      Serial.println("Dentro del primer if");
      if (gps.location.isValid())
      {
        Serial.println("Dentro del segundo if");
        latitude = gps.location.lat();
        lat_str = String(latitude , 6);
        longitude = gps.location.lng();
        lng_str = String(longitude , 6);
        sats = gps.satellites.value();
        sats_str = String(sats);

        //digitalWrite(12, HIGH); 

        lcd.setCursor(0,0);
        lcd.print("LAT: " + lat_str);
        
        lcd.setCursor(1,0);
        lcd.print("LONG: " + lng_str);
        
        lcd.setCursor(2,0);
        lcd.print("SATS: " + sats_str);

      }else{
        lcd.print("Buscando satélites...");
        Serial.println("Buscando satélites");
		    //digitalWrite(12, LOW);        
      }
    } else{
      lcd.print("Sin GPS");
      Serial.println("Sin GPS");
    }

    delay(1000);
    Serial.println("Limpiando pantalla");
    lcd.clear();
  }
}
