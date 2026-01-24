#include <IRac.h>
#include <IRmacros.h>
//#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRtext.h>
#include <IRtimer.h>
#include <IRutils.h>
#include <i18n.h>
#include <ir_Panasonic.h>

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include "libbot.h"

//****************************************************
// CONFIGURACION PARA LA RADIO
// #ifndef WIFI_SSID
// #define WIFI_SSID "Sysbot"
// #define WIFI_PASSWORD "sysbot123"
// #endif
// #define BOT_TOKEN ""
// #define ID_Chat ""//ID_Chat se obtiene de telegram
//****************************************************

//****************************************************
//  CONFIGURACION PARA PRUEBAS
#ifndef WIFI_SSID
#define WIFI_SSID "PELOTERO"
#define WIFI_PASSWORD "laclavees1981"
#endif
#define BOT_TOKEN "6155203747:AAHXbcoaD_Axnoor4fBVeJQW1fVG4BXjOmk"
#define ID_Chat "677317280"//ID_Chat se obtiene de telegram
//****************************************************

#define PIN_RELE 0
#define PIN_TEMP 1
#define PIN_IR 2

WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);

IRsend emisor_ir(PIN_IR);
#define AC_ON_16 1686110244
#define AC_OFF 1686110212
#define BITS_IR 48

unsigned long tiempoAnterior;  //última vez que se realizó el análisis de mensajes
const unsigned long tiempo = 1000; //tiempo medio entre mensajes de escaneo
int inicio = 1;
bool fin = false;

X509List cert(TELEGRAM_CERTIFICATE_ROOT);

String chat_id;
String text;
String msg;

void mensajesNuevos(int numerosMensajes) {
  for (int i = 0; i < numerosMensajes; i++) {
    chat_id = bot.messages[i].chat_id;
    text = bot.messages[i].text;
    msg;

    int opcion = validarOpcion(text);
    switch (opcion){
      case 1:
      {
        msg = "Temperatura: " + String(readTemp());
        bot.sendMessage(chat_id, msg);  //Enviamos el mensaje
        break;
      }
      case 2:
      {
        bot.sendMessage(chat_id, "Encendiendo el AC");  //Enviamos el mensaje
        emisor_ir.send(PANASONIC, AC_ON_16, BITS_IR);
        break;
      }
      case 3:
      {
        bot.sendMessage(chat_id, msg, "Apagando el AC");
        emisor_ir.send(PANASONIC, AC_OFF, BITS_IR);
        break;
      }
      case 4:
      {
        fin = true;
        break;
      }
      case 0:
      {
        msg = generateMenu();
        bot.sendMessage(chat_id, msg, "");
        break;
      }
      default:
      {
        msg = "no comprendo ese comando \U0001F611 \n";
        bot.sendMessage(chat_id, msg );
        break;
      }
    }
  }
}

void setup() {
  //Definiciones de pines
  pinMode(PIN_RELE, OUTPUT);
  pinMode(PIN_TEMP, INPUT);

  // Intenta conectarse a la red wifi
  secured_client.setInsecure();
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  configTime(0, 0, "pool.ntp.org");
  secured_client.setTrustAnchors(&cert);
  while (WiFi.status() != WL_CONNECTED) {
    //Serial.print(".");
    delay(500);
  }
  
  if (inicio == 1) {
    String msg = "Hola soy el bot de la radio \n";
    msg += "El sistema está preparado \U0001FAE1 \n";
    bot.sendMessage(ID_Chat, msg, "");  //Enviamos un mensaje a telegram para informar que el sistema está listo}
    msg = generateMenu();
    bot.sendMessage(ID_Chat, msg, "");
    inicio = 0;
  }
}

void loop() {
  //Verifica si hay datos nuevos en telegram cada 1 segundo
  if (millis() - tiempoAnterior > tiempo) {
    int numerosMensajes = bot.getUpdates(bot.last_message_received + 1);
    Serial.println("Mensajes: " + String(numerosMensajes));
    while (numerosMensajes) {
      mensajesNuevos(numerosMensajes);
      numerosMensajes = bot.getUpdates(bot.last_message_received + 1);
    }
    tiempoAnterior = millis();
  }
  if(fin)
  {
    apagaRadio();
    fin = false;
  }
}

void apagaRadio()
{
  bot.sendMessage(chat_id, "Apagando la radio");
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(PIN_RELE, HIGH);
  delay(2000);
  digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(PIN_RELE, LOW);
}

int readTemp()
{
  int sensor = 0;
  float temp = 0;
  int suma = 0;
  for (int i=0; i<5; i++)
  {
    sensor = analogRead(PIN_TEMP);
    temp = ((sensor * 5000.0) / 1023) / 10;
    suma = temp + suma;
    delay(500);
  }
  return (ceil(temp/5));
}


