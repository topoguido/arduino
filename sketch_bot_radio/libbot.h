#include <WString.h>

int validarOpcion(String text){
  if(text == "/1"){
    return 1;
  }
  if(text == "/2"){
    return 2;
  }
  if(text == "/3"){
    return 3;
  }
  if(text == "/4"){
    return 4;
  }
  if(text == "/0"){
    return 0;
  }
  return 0;
}

String generateMenu(){
  String ayuda = "Estas son tus opciones.\n\n";
  ayuda += "/1: Chequear la temperatura \n";
  ayuda += "/2: Encender el AC \n";
  ayuda += "/3: Apagar el AC \n";
  ayuda += "/4: Apagar la radio \n";
  ayuda += "/0: Imprime este menú \n";
  //ayuda += "Recuerda que el sistema distingue entre mayuculas y minusculas \n";
  return ayuda;
}