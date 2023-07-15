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

uint8_t col = 0;
uint8_t row = 0;

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

    //myPrint(col, row, String("Dentro del while"), false, true);

    if (gps.encode(ss.read()))
    { 
      myPrint(col, row, String("GPS activo"), false, true);
      delay(500);
      if (gps.location.isValid())
      {
        myPrint(col, row, String("Ubicación válida"), false, true);
        delay(500);

        latitude = gps.location.lat();
        lat_str = String(latitude , 6);
        longitude = gps.location.lng();
        lng_str = String(longitude , 6);
        sats = gps.satellites.value();
        sats_str = String(sats);

        //digitalWrite(12, HIGH); 

        //lcd.setCursor(0,0);
        //lcd.print("LAT: " + lat_str);

        myPrint(col, row, lat_str, false, true);
        
        //lcd.setCursor(1,0);
        //lcd.print("LONG: " + lng_str);
        
        myPrint(col, row, lng_str, true, true);

        //lcd.setCursor(2,0);
        //lcd.print("SATS: " + sats_str);

        myPrint(col, row, sats_str, false, true);

      }else{
        myPrint(col, row, String("Buscando satélites"), false, true);
		    //digitalWrite(12, LOW);        
      }
    } else{
      myPrint(col, row, String("Sin GPS"), false, true);
    }

    delay(1000);
    lcd.clear();
  }
}

void myPrint(uint8_t col, uint8_t row, String message, bool newLine, bool serial){
  if(!message.isEmpty()){
    
    if(!newLine){
      lcd.clear();
      lcd.setCursor(col, row);  
    } else{
      col = 0;
      row++;
    }
    
    lcd.print(message);
    if(serial){
      Serial.println(message);
    }
    if(!newLine){
      row = row + message.length();
    } 

  }


}
