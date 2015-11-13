/*Este programa le permite encender la TV con nuestro Arduino. 
El primer paso es capturar el codigo RAW emitida por nuestro 
control remoto, a través del programa IR_DECODE.
Ahora tenemos que tomar el código RAW, elimine el primer valor, 
convertir los signos "-" en espacio y poner comas entre los distintos valores.
Por ej. para el mando SONY , IR_DECODE nos decodifica el boton ON/OFF como:
Raw (26): -25500 2450 -500 1300 -500 650 -550 1250 -500 700 -500 1250 -550 650 -500 700 -500 1250 -550 650 -550 650 -500 700 -500 650 
la secuencia quedaría como : 
2450,500,1300,500,650,550,1250,500,700,500,1250,550,650,500,700,500,1250,550,650,550,650,500,700,500,650 

El emisor es un LED IR conectado al pin 3 a traves de una resistencia de 100 Ohm
*/

#include "IRremote.h"
 
IRsend irsend;
 
void setup()
{
  Serial.begin(9600);
}
 
unsigned int ON[25]={2450,500,1300,500,650,550,1250,500,700,500,1250,550,650,500,700,500,1250,550,650,550,650,500,700,500,650};
 
void loop() {
 irsend.sendRaw(ON,25,38);
 Serial.println("encendiendo TV");
 delay(10);
 }
