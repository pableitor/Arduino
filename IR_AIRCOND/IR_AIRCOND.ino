/*Este programa le permite encender el aire acondicionado con nuestro Arduino. 
El primer paso es capturar el codigo RAW emitida por nuestro 
control remoto, a través del programa IR_AIRCON_DECODE.
Ahora tenemos que tomar el código RAW, elimine el primer valor, 
convertir los signos "-" en espacio y poner comas entre los distintos valores.
Por ej. para el mando SONY , IR_DECODE nos decodifica el boton ON/OFF como:
Raw (26): -25500 2450 -500 1300 -500 650 -550 1250 -500 700 -500 1250 -550 650 -500 700 -500 1250 -550 650 -550 650 -500 700 -500 650 
la secuencia quedaría como : 
2450,500,1300,500,650,550,1250,500,700,500,1250,550,650,500,700,500,1250,550,650,550,650,500,700,500,650 

A traves del monitor serie le enviamos diferentes comandos:
0 - Apagar
1 - Encender
2 - Subir temperatura
3 - Enfriar
4 - Potencia ventilador
5 - Parar/Mover difusor
6 - Modo bomba de calor (enfriar, calentar, 
El emisor es un LED IR conectado al pin 3 */

#include "IRremote.h"

IRsend irsend;
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

void setup()
{
  Serial.begin(9600);
    // reserve 10 bytes for the inputString:
  inputString.reserve(10);
}

int khz=38; //NB Change this default value as neccessary to the correct modulation frequency
// ON and 2O C° with 1 FAN heat
unsigned ON[] = {9000,4350,700,1550,650,1600,600,500,650,450,650,450,650,500,600,500,600,1600,650,1600,600,1600,650,450,650,1600,600,500,650,450,650,450,650,500,600,1600,650,450,650,500,600,500,600,500,650,450,650,500,600,500,650,450,650,450,650,500,600,500,650,450,650,450,650,500,600,500,650,450,650,1550,650,500,600,500,650,450,650,450,650,1600,650,450,650,500,600,500,600,1600,650,1550,650,1600,600,1600,650,450,650,1600,600};

unsigned heat[] = {8850,4450,650,1600,600,1600,550,550,600,550,600,500,650,500,500,600,600,1600,600,1600,600,1650,600,500,650,1550,600,550,550,550,550,550,550,550,650,1600,600,500,550,550,550,550,600,550,550,600,600,450,650,500,600,500,600,550,500,550,650,450,650,500,600,550,500,550,650,500,600,500,600,1600,650,450,600,550,600,500,550,550,650,1600,550,600,500,550,550,550,650,1600,550,1650,650,1550,600,1650,550,600,600,1550,650};
// OFF the A/C
unsigned OFF[] = {8950,4400,600,1550,700,1550,650,500,600,500,650,450,650,450,650,500,600,1600,650,1600,600,1600,650,450,650,1550,650,500,650,450,650,500,600,500,600,1600,650,500,600,500,600,500,600,500,650,450,650,500,600,500,650,450,650,500,600,500,600,500,650,1550,650,500,600,500,600,500,650,500,600,1550,650,500,600,500,650,500,600,500,600,1600,650,450,650,1600,600,1600,650,450,650,1550,650,1600,650,450,650,1550,650,1600,650};
// ON and 23° with 2 FAN cold
unsigned cold[] = {8900,4450,550,1650,600,1650,600,500,600,500,550,600,500,600,600,500,550,1650,550,1700,600,1600,550,600,550,1650,600,500,550,550,600,550,500,600,550,1650,550,550,550,600,500,600,550,550,550,600,500,600,550,550,550,550,550,600,500,600,550,550,550,550,550,600,500,600,550,550,550,550,550,1700,500,600,550,550,550,550,550,600,500,1700,550,600,500,1700,550,1650,550,550,550,1700,500,600,550,550,550,1700,500,1700,550};
// ON and 20 C° with 1 FAN heat and SWING
unsigned fan[] = {9000,4350,650,1600,600,1600,650,450,650,450,650,500,600,500,650,450,650,1550,650,1600,650,1550,650,500,600,1600,650,450,650,500,600,500,600,500,650,1550,650,500,600,500,650,450,650,450,650,500,650,450,650,450,650,500,600,500,600,500,650,450,650,450,650,500,600,500,650,450,650,1600,600,1600,650,450,650,450,650,500,600,500,650,1550,650,500,600,1600,650,1550,650,500,600,1600,650,1550,650,500,600,1600,650,1550,650};
// ON and 23° with 2 FAN cold and SWING
unsigned swing[] = {8900,4450,550,1650,550,1650,600,550,550,550,550,550,550,550,550,600,550,1650,550,1650,600,1650,550,550,550,1650,600,550,550,550,550,550,550,550,600,1650,550,550,550,550,550,600,550,550,550,550,600,550,550,550,550,1650,550,550,550,600,550,550,550,1650,550,550,550,600,550,550,550,1650,550,550,600,550,550,550,550};

unsigned mode[] = {8900,4450,550,1650,600,1650,550,550,550,600,500,600,550,550,550,550,550,1650,550,1700,550,1650,550,600,550,1650,550,550,550,550,550,600,550,550,550,1650,550,550,550,600,550,550,550,550,600,550,550,550,550,550,550,550,550,600,550,550,550,600,500,550,550,600,550,550,550,600,500,550,550,600,550,550,550,600,500,600,500,550,600,550,550,600,500,550,600,550,550,550,550,600,500,1700,550,550,550,600,500,550,550};

void loop() {

  if (stringComplete) {
    if (inputString=="1"){
        Serial.println("ON");
        irsend.sendRaw(ON, sizeof(ON)/sizeof(int), khz);
    } 
    else 
    if (inputString=="0"){  
        Serial.println("OFF");
        irsend.sendRaw(OFF, sizeof(OFF)/sizeof(int), khz);
    }
    else 
    if (inputString=="2"){  
        Serial.println("heat");
        irsend.sendRaw(OFF, sizeof(heat)/sizeof(int), khz);
    }
    else 
    if (inputString=="3"){  
        Serial.println("cold");
        irsend.sendRaw(cold, sizeof(cold)/sizeof(int), khz);
    }    
    else 
    if (inputString=="4"){  
        Serial.println("fan");
        irsend.sendRaw(cold, sizeof(fan)/sizeof(int), khz);
    }    
    else 
    if (inputString=="5"){  
        Serial.println("swing");
        irsend.sendRaw(cold, sizeof(swing)/sizeof(int), khz);
    }    
    else 
    if (inputString=="6"){  
        Serial.println("mode");
        irsend.sendRaw(cold, sizeof(mode)/sizeof(int), khz);
    }    
    else 

    Serial.println("ERROR");
    // clear the string:
    inputString = "";
    stringComplete = false;
    }
    
    delay(500);
}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:

    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    } else
        inputString += inChar;
  }
}
