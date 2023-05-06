#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include "libbot.h"

#ifndef WIFI_SSID
#define WIFI_SSID "PELOTERO"
#define WIFI_PASSWORD "laclavees1981"
#endif

#define BOT_TOKEN "6155203747:AAHXbcoaD_Axnoor4fBVeJQW1fVG4BXjOmk"
#define ID_Chat "677317280"//ID_Chat se obtiene de telegram
#define PIN_RELE 0

WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);

unsigned long tiempoAnterior;  //última vez que se realizó el análisis de mensajes
const unsigned long tiempo = 1000; //tiempo medio entre mensajes de escaneo
int inicio = 1;

X509List cert(TELEGRAM_CERTIFICATE_ROOT);

void mensajesNuevos(int numerosMensajes) {
  for (int i = 0; i < numerosMensajes; i++) {
    String chat_id = bot.messages[i].chat_id;
    String text = bot.messages[i].text;
    String msg;

    int opcion = validarOpcion(text);
    switch (opcion){
      case 1:
      {
        bot.sendMessage(chat_id, "Encendiendo");  //Enviamos el mensaje
        //Serial.println("prendiendo led");
        delay(500);
        digitalWrite(LED_BUILTIN, HIGH);
        digitalWrite(PIN_RELE, LOW);
        break;
      }
      case 2:
      {
        bot.sendMessage(chat_id, "Apagando");  //Enviamos el mensaje
        //Serial.println("apagando led");
        delay(500);
        digitalWrite(LED_BUILTIN, LOW);
        digitalWrite(PIN_RELE, HIGH);
        delay(2000);
        digitalWrite(LED_BUILTIN, HIGH);
        digitalWrite(PIN_RELE, LOW);
        break;
      }
      case 3:
      {
        if(digitalRead(PIN_RELE) == HIGH){
          msg = "Estado del relé: *Encendido*";
        }
        else{
          msg = "Estado del relé: *Apagado*";
        }
        bot.sendMessage(chat_id, msg, "");
        break;
      }
      case 9:
      {
        String ayuda = generateMenu();
        bot.sendMessage(chat_id, ayuda, "");
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
  pinMode(LED_BUILTIN, OUTPUT); 
  digitalWrite(LED_BUILTIN, HIGH);
  pinMode(PIN_RELE, OUTPUT);
  //Serial.begin(115200);
  // Intenta conectarse a la red wifi
  //Serial.print("Conectando a la red ");
  //Serial.print(WIFI_SSID);
  secured_client.setInsecure();
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  configTime(0, 0, "pool.ntp.org");
  secured_client.setTrustAnchors(&cert);
  while (WiFi.status() != WL_CONNECTED) {
    //Serial.print(".");
    delay(500);
  }
  
  if (inicio == 1) {
    //Serial.println("Sistema preparado");
    String msg = "Hola soy el topobot \n";
    msg += "El sistema está preparado \U0001FAE1 \n";
    bot.sendMessage(ID_Chat, msg, "");  //Enviamos un mensaje a telegram para informar que el sistema está listo}
    msg = generateMenu();
    bot.sendMessage(ID_Chat, msg, "");
    //Serial.println("Mensaje enviado");
    inicio = 0;
  }
}

void loop() {
  //Verifica si hay datos nuevos en telegram cada 1 segundo
  if (millis() - tiempoAnterior > tiempo) {
    int numerosMensajes = bot.getUpdates(bot.last_message_received + 1);

    while (numerosMensajes) {
      //Serial.println("Comando recibido");
      mensajesNuevos(numerosMensajes);
      numerosMensajes = bot.getUpdates(bot.last_message_received + 1);
    }
    tiempoAnterior = millis();
  }
}


