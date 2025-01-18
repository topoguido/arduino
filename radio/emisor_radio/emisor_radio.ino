#include <IRremote.h>
//decode_results IR;
IRsend emisor_ir;
unsigned long value_on_16 = 1686110244;
unsigned long value_off = 1686110212;

void setup() {
  Serial.begin(9600);
  emisor_ir.bits = 48;
}

void loop() {
  delay(5000);
  

  IR.value = 16753245;
  //Serial.println("Lectura - Tipo: " + String(IR.decode_type) + " Bits: " + String(IR.bits) + " Value: " + String(IR.value));
  Emisor_IR.send_PANASONIC(IR.value, IR.bits);
  
  delay(3000);
  exit(0);
} 
