#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "webpage.h"

#ifndef LED
#define LED 0
#define DIR 5 
#endif

String state, steps;
uint8_t steps = 0;

const char* ssid = "PELOTERO";
const char* password = "laclavees1981";

ESP8266WebServer server(80);

void handleRoot() 
{
	String s = webpage;
	server.send(200, "text/html", s);
}

void valve_state() 
{
	server.send(200, "text/plane", state);
}

void valve_open() 
{
  steps = server.arg("steps");
  digitalWrite(LED,HIGH); //LED ON
  state = "OPEN";
  Serial.println("Abriendo...");
  server.send(200, "text/plane", state);
}

void valve_close()
{
  steps = server.arg("steps");
  digitalWrite(LED,LOW); //LED OFF
  state = "CLOSE";
  Serial.println("Cerrando...");
  server.send(200, "text/plane", state);
}

void setup(void)
{
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");
  pinMode(LED,OUTPUT); 
  while (WiFi.status() != WL_CONNECTED) {Serial.print("Connecting...");}
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/valve_open", valve_open);
  server.on("/valve_close", valve_close);
  server.begin();
}

void loop(void)
{
  server.handleClient();
}

void moveEngine(uint8_t steps)
{
  digitalWrite(DIR, HIGH);
  for(int i = 0; i < steps; i++)
  {
    digitalWrite(STEP, HIGH);
    delay(delayStep);
    digitalWrite(STEP, LOW);
    delay(delayStep);
  }
}