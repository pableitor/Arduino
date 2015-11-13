#include <SoftwareSerial.h>
#define baudrate 9600
SoftwareSerial BT1(13, 12); // pines RX | TX  del puerto serie virtual

void setup()
  {  Serial.begin(baudrate); //inicializa el puerto serie HW
     BT1.begin(baudrate); // init virtual serial port
  }

void loop()
  {  if (BT1.available()) //si hay datos en el puerto serie virtual
         { char c = BT1.read() ; //lee puerto serie virtual
           Serial.print(c); //lo imprime por el puerto serie HW conectado a la consola
         }
     if (Serial.available()) // si hay datos en la consola
         {  char c = Serial.read(); //lee la consola
            BT1.print(c); //los envia al puerto serie virtual
         }
   }
