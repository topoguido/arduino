#include "webpage.h"
/* Create a WiFi access point and provide a web server on it. */
#define __DEBUG__
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <TinyGPSPlus.h>
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#ifndef APSSID
#define APSSID "GPS-AP"
#define APPSK "thereisnospoon"
#endif

/* Set these to your desired credentials. */
const char *ssid = APSSID;
const char *password = APPSK;

ESP8266WebServer server(80);

TinyGPSPlus gps;
static const int RXPin = 14, TXPin = 12;
SoftwareSerial ss(RXPin, TXPin);// WEMOS: GPIO14 and GPIO12 (PIN D5 and D6)

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET -1

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

uint8_t col = 0;
uint8_t row = 0;
uint8_t TEXT_SIZE = 1;

String lat_str , lng_str, alt_str, sats_str, date_str, time_str;

TinyGPSCustom pdop(gps, "GPGSA", 15); // $GPGSA sentence, 15th element
TinyGPSCustom hdop(gps, "GPGSA", 16); // $GPGSA sentence, 16th element
TinyGPSCustom vdop(gps, "GPGSA", 17); // $GPGSA sentence, 17th element

String header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";

/* Just a little test message.  Go to http://192.168.4.1 in a web browser
   connected to this access point to see it.
*/


void setup() {
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  ss.begin(9600);
  Wire.begin();
  Serial.begin(115200);
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

  Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");
}
void handleRoot() {
  String str = webpage;
  str.replace("%lat%", lat_str);
  str.replace("%long%", lng_str);
  str.replace("%sats%", sats_str);
  str.replace("%alt%", alt_str);
  str.replace("%date%", date_str);
  str.replace("%time%", time_str);
  Serial.println("Sending data");
  server.send(200, "text/html", str);
}
void loop() {
  
  // GPS:
  genGPSData();
  server.handleClient();
}

void genGPSData()
{
  if (gps.altitude.isUpdated() || gps.satellites.isUpdated() ||
    pdop.isUpdated() || hdop.isUpdated() || vdop.isUpdated())
  { 
    if(TEXT_SIZE == 1)
    {
      lat_str = String(gps.location.lat(), 6);
      lng_str = String(gps.location.lng(), 6);
      alt_str = String(gps.altitude.meters());
      sats_str = String(gps.satellites.value());
      date_str =  genDate();
      time_str = genTime();
      
      myPrint(col, row, "Lat: " + lat_str, false, true);
      myPrint(col, row, "Long: " + lng_str, true, true);
      myPrint(col, row, "Sat:" + sats_str + "  Alt:" + alt_str + " m", true, true);
      myPrint(col, row, date_str + " - " + time_str, true, true);
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
  if(gps.location.isValid()){
    digitalWrite(13, HIGH);
  } else {
    digitalWrite(13, LOW);
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
      row++;
    } 
  }
}

String genDate()
{

  String str;

  if (gps.date.isValid())
  {
    str = String(gps.date.day());
    str = str + F("/");
    str = str + String(gps.date.month());
    str = str + F("/");
    str = str + String(gps.date.year());
  } else {
    str = str + "No date";
  }
  return str;
}

String genTime()
{
  String str;
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) str = F("0");
    str = str + String(gps.time.hour()) + ":";

    if (gps.time.minute() < 10) str = str + F("0");
    str = str + String(gps.time.minute()) + F(":");
    
    if (gps.time.second() < 10) str = str + F("0");
    str = str + String(gps.time.second());

  }  else {
    str = str + "No time";
  }
  return str;
}
