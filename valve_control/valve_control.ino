#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "index.h"

#ifndef LED
#define LED 0
#endif

const char* ssid = "PELOTERO";
const char* password = "laclavees1981";

ESP8266WebServer server(80);

void handleRoot() 
{
	String s = webpage;
	server.send(200, "text/html", s);
}

void sensor_data() 
{
	int a = analogRead(A0);
	int temp= a/4.35;
	String sensor_value = String(temp);
	server.send(200, "text/plane", sensor_value);
}

void valve_state() 
{
 String state = "CLOSE";
 String act_state = server.arg("state");
 if(act_state == "1")
 {
  digitalWrite(LED,HIGH); //LED ON
  state = "OPEN";
  Serial.println("Abriendo...");
 }
 else
 {
  digitalWrite(LED,LOW); //LED OFF
  state = "CLOSE";
  Serial.println("Cerrando...");
 }
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
  server.on("/valve_state", valve_state);
  server.on("/adcread", sensor_data);
  server.begin();
}

void loop(void)
{
  server.handleClient();
}