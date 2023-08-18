#include "webpage.h"
#include "graph_matria.h"
#include "data.h"
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
#include <ArduinoJson.h>

#ifndef APSSID
#define APSSID "GPS-AP"
#endif

/* Set these to your desired credentials. */
const char *ssid = APSSID;

ESP8266WebServer server(80);

static const int minSats = 5;
TinyGPSPlus gps;
static const int RXPin = 14, TXPin = 12;
SoftwareSerial ss(RXPin, TXPin);// WEMOS: GPIO14 and GPIO12 (PIN D5 and D6)

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

uint8_t col = 0;
uint8_t row = 0;
uint8_t TEXT_SIZE = 2;

gpsData gpsD;

TinyGPSCustom pdop(gps, "GPGSA", 15); // $GPGSA sentence, 15th element
TinyGPSCustom hdop(gps, "GPGSA", 16); // $GPGSA sentence, 16th element
TinyGPSCustom vdop(gps, "GPGSA", 17); // $GPGSA sentence, 17th element

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

  display.setTextSize(TEXT_SIZE);
  display.setTextColor(WHITE);
  display.setCursor(col, row);

  printLogo();
 
  Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.on("/", handleRoot);
  server.on("/read",  genJson);
  server.begin();
  Serial.println("HTTP server started");
}
void handleRoot() {
  String str = webpage;
  server.send(200, "text/html", str);
}

void genJson()
{
  StaticJsonDocument<130> doc;
  doc["lat"] = gpsD.latitud;
  doc["long"] = gpsD.longitud;
  doc["alt"] = gpsD.alt;
  doc["sats"] = gpsD.sats;
  doc["dateTime"] = gpsD.dateTime;

  char json[120];
  serializeJson(doc, json, sizeof(json));

  Serial.println(json);
  server.send(200, "text/json", json);
}

void loop() {
  server.handleClient();
  genGPSData();
}

void genGPSData()
{
  if (gps.altitude.isUpdated() || gps.satellites.isUpdated() ||
    pdop.isUpdated() || hdop.isUpdated() || vdop.isUpdated())
  { 
    if(gps.satellites.value() > minSats)    
    {
      digitalWrite(13, HIGH);
      gpsD.latitud = gps.location.lat();
      gpsD.longitud = gps.location.lng();
      gpsD.sats = gps.satellites.value();
      gpsD.alt = gps.altitude.meters();
      gpsD.dateTime = genDate() + " - " + genTime();
      
      if(TEXT_SIZE == 1)
      {
        myPrint(col, row, "Lat: " + String(gpsD.latitud, 6), false, false);
        myPrint(col, row, "Long: " + String(gpsD.longitud, 6), true, false);
        myPrint(col, row, "Sat:" + String(gpsD.sats) + "  Alt:" + String(gpsD.alt) + " m", true, false);
        myPrint(col, row, gpsD.dateTime, true, false);
        printSerialGPS();
      }
      if(TEXT_SIZE == 2)
      {
        myPrint(col, row, String(gpsD.latitud, 6), false, false);
        myPrint(col, row, String(gpsD.longitud, 6), true, false);
        myPrint(col, row, "", true, false);
        myPrint(col, row, "Sats: " + String(gpsD.sats), true, false);
        myPrint(col, row, "Alt: "  + String(gpsD.alt), true, false);
        //printSerialGPS();
      }
    } else {
      digitalWrite(13, LOW);
      Serial.println("Low sats: " + String(gps.satellites.value()));
      printSat();
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
      Serial.print(message);
    }
    if(!newLine){
      row = row + message.length();
    } 
  }
}

void printSerialGPS()
{
  Serial.println();
  Serial.print("Lat: " + String(gps.location.lat(), 6));
  Serial.print(" | ");    
  Serial.print("Long: " + String(gps.location.lng(), 6));
  Serial.print(" | ");    
  Serial.print("Sats: " + String(gps.satellites.value()));
  Serial.print(" | ");    
  Serial.print("Alt: ") + String(gps.altitude.meters(), 0);
  Serial.print(" | ");    
  Serial.print("Date: " + genDate());
  Serial.print(" | ");    
  Serial.print("Time: " + genTime());
}

void printLogo()
{
  display.clearDisplay();
  display.drawBitmap(0, 0, logo, 128, 64, WHITE);
  display.display();
  delay(4000);
  display.clearDisplay();
  myPrint(16, 20, "Iniciando", false, true);
  display.display();
  delay(4000);
}

void printSat()
{
  display.clearDisplay();
  display.drawBitmap(30, 0, sat, 64, 64, WHITE); 
  display.display();
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
    str = str + String((gps.time.hour())) + ":";

    if (gps.time.minute() < 10) str = str + F("0");
    str = str + String(gps.time.minute()) + F(":");
    
    if (gps.time.second() < 10) str = str + F("0");
    str = str + String(gps.time.second());

  }  else {
    str = str + "No time";
  }
  return str;
}
