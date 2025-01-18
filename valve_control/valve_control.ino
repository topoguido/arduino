#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "webpage.h"

#define LED 1
#define DIR 0
#define STEP 2 

String state, steps;

const char* ssid = "PELOTERO";
const char* password = "laclavees1981";

int delayStep = 3;

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
  state = "ABIERTA";
  Serial.println("Abriendo...");
  Serial.println(steps);
  moveEngine(steps.toInt());
  server.send(200, "text/plane", state);
}

void valve_close()
{
  steps = server.arg("steps");
  digitalWrite(LED,LOW); //LED OFF
  state = "CERRADA";
  Serial.println("Cerrando...");
  Serial.println(steps);
  moveEngine(steps.toInt());
  server.send(200, "text/plane", state);
}

void setup(void)
{
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  Serial.println("");
  pinMode(LED,OUTPUT); 
  pinMode(DIR,OUTPUT); 
  pinMode(STEP,OUTPUT); 
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
    digitalWrite(steps, HIGH);
    delay(delayStep);
    digitalWrite(steps, LOW);
    delay(delayStep);
  }
}