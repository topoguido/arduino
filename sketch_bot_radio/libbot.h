#include <WString.h>

int validarOpcion(String text){
  // if(text == "/on"){
  //   return 1;
  // }
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

String generateMenu(){
  String ayuda = "Estas son tus opciones.\n\n";
  //ayuda += "/on: para encender la radio \n";
  ayuda += "/off: para apagar la radio \n";
  ayuda += "/stat: para saber el estado del led \n";
  ayuda += "/ayuda: Imprime este menú \n";
  ayuda += "Recuerda que el sistema distingue entre mayuculas y minusculas \n";
  return ayuda;
}