
// GPS-MODUL
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
TinyGPSPlus gps;
SoftwareSerial ss(4, 5);// GPIO4 und GPIO5 (PIN D1 und D2)
float latitude , longitude;
String lat_str , lng_str;

//=======================================================================
//                    Setup
//=======================================================================
 
void setup() {
    //LEDs:
  pinMode(12, OUTPUT); digitalWrite(12, LOW);//LED an D6: Valid GPS Data
   
  // Serial:
  Serial.begin(115200);
 
  // SoftwareSerial:
  ss.begin(9600);
}
 
//=======================================================================
//                    Main Program Loop
//=======================================================================

void loop() {

  // GPS:
  while (ss.available() > 0)
    if (gps.encode(ss.read()))
    { 
      if (gps.location.isValid())
      {
		Serial.println("Location is valid ...");
		digitalWrite(12, HIGH); 
        latitude = gps.location.lat();
        lat_str = String(latitude , 6);
		Serial.print("LAT:");
		Serial.println(lat_str);        
        longitude = gps.location.lng();
        lng_str = String(longitude , 6);
		Serial.print("LNG:"); 
		Serial.println(lng_str);       
      }else{
		digitalWrite(12, LOW);        
      }
    }


}

