#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

#ifndef WIFI_SSID
#define WIFI_SSID "PELOTERO"
#define WIFI_PASSWORD "laclavees1981"
#endif

#define BOT_TOKEN "6155203747:AAHXbcoaD_Axnoor4fBVeJQW1fVG4BXjOmk"
const unsigned long tiempo = 1000; //tiempo medio entre mensajes de escaneo
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);
unsigned long tiempoAnterior;  //última vez que se realizó el análisis de mensajes
int inicio = 1;
#define ID_Chat "677317280"//ID_Chat se obtiene de telegram

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
        digitalWrite(LED_BUILTIN, LOW);
        digitalWrite(2, HIGH);
        break;
      }
      case 2:
      {
        bot.sendMessage(chat_id, "Apagando");  //Enviamos el mensaje
        //Serial.println("apagando led");
        delay(500);
        digitalWrite(LED_BUILTIN, HIGH);
        digitalWrite(2, LOW);
        break;
      }
      case 3:
      {
        if(digitalRead(2) == HIGH){
          msg = "Estado del led: *Encendido*";
        }
        else{
          msg = "Estado del led: *Apagado*";
        }
        bot.sendMessage(chat_id, msg, "");
        break;
      }
      case 9:
      {
        String ayuda = "Bienvenido al sistema de control, "
                       ".\n";
        ayuda += "Estas son tus opciones.\n\n";
        ayuda += "/on: para encender el led \n";
        ayuda += "/off: para apagar el led \n";
        ayuda += "/stat: para saber el estado del led \n";
        ayuda += "/ayuda: Imprime este menú \n";
        ayuda += "Recuerda que el sistema distingue entre mayuculas y minusculas \n";
        bot.sendMessage(chat_id, ayuda, "");
        break;
      }
      default:
      {
        bot.sendMessage(chat_id, "no comprendo ese comando :( \n" );
        break;
      }
    }
  }
}

int validarOpcion(const String &text){
  if(text == "/on"){
    return 1;
  }
  if(text == "/off"){
    return 2;
  }
  if(text == "/stat"){
    return 3;
  }
  if(text == "/ayuda"){
    return 9;
  } else {
    return 0;
  }

}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT); 
  digitalWrite(LED_BUILTIN, HIGH);
  pinMode(2, OUTPUT);
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
  //Serial.print("\nConectado a la red wifi. Dirección IP: ");
  //Serial.println(WiFi.localIP());
  if (inicio == 1) {
    //Serial.println("Sistema preparado");
    bot.sendMessage(ID_Chat, "Sistema preparado!!!, escribe /ayuda para ver las opciones", "");  //Enviamos un mensaje a telegram para informar que el sistema está listo}
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


