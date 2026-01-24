#include <IRremote.h>
IRrecv Receptor_IR(5);
decode_results IR;
IRsend Emisor_IR;
#define pin_pulsador 2
#define pin_led 6
unsigned long millis_anterior_pulsacion = 0;
uint8_t contador_pulsacion = 0;
void setup() {
  Serial.begin(9600);
  pinMode(pin_led, OUTPUT);
  pinMode(pin_pulsador, INPUT_PULLUP);
  Receptor_IR.blink13(true);
}
void tiempo_pulsacion() {
  unsigned long millis_actual_pulsacion = millis();
  if (millis_actual_pulsacion - millis_anterior_pulsacion > 1000UL) {
    millis_anterior_pulsacion = millis_actual_pulsacion;
    contador_pulsacion++;
  }
}
void loop() {
  if (digitalRead(pin_pulsador) == LOW) {
    delay(15);
    if (!digitalRead(pin_pulsador)) {
      tiempo_pulsacion();
      if (contador_pulsacion == 3) { // 3 = Tiempo de pulsacion
        contador_pulsacion = 0;
        //
        //------------------------------------------------------
        digitalWrite(pin_led, HIGH);
        Receptor_IR.enableIRIn();
        while (!Receptor_IR.decode(&IR));
        digitalWrite(pin_led, LOW);
        Serial.println("Lectura - Tipo: " + String(IR.decode_type) + " Bits: " + String(IR.bits) + " Value: " + String(IR.value));
        
        //------------------------------------------------------
        //
      }
    }
  } else {
    if (contador_pulsacion == 1) {
      contador_pulsacion = 0;
      //
      //------------------------------------------------------
      digitalWrite(pin_led, HIGH);
      switch (IR.decode_type) {
        case NEC: Emisor_IR.sendNEC(IR.value, IR.bits); break ;
        //case NEC: Emisor_IR.sendNEC(0x16753245, 32); break ;
        case SONY: Emisor_IR.sendSony(IR.value, IR.bits); break ;
        case RC5:  Emisor_IR.sendRC5(IR.value, IR.bits); break ;
        case RC6: Emisor_IR.sendRC6(IR.value, IR.bits); break ;
        case DISH: Emisor_IR.sendDISH(IR.value, IR.bits); break ;
        case SHARP: Emisor_IR.sendSharp(IR.value, IR.bits); break ;
        case SAMSUNG: Emisor_IR.sendSAMSUNG(IR.value, IR.bits); break ;
        case LG: Emisor_IR.sendLG(IR.value, IR.bits); break ;
        case PANASONIC: Emisor_IR.sendPanasonic(IR.value, IR.bits); break ;
        case DENON: Emisor_IR.sendDenon(IR.value, IR.bits); break ;
        default:
        case UNKNOWN: delay(100); digitalWrite(pin_led, LOW);
          delay(100); digitalWrite(pin_led, HIGH); break ;
      }
      delay(100);
      digitalWrite(pin_led, LOW);
      //--------------------------------------------------------
      //
    } else {
      contador_pulsacion = 0;
    }
  }
  delay(100);
} 
