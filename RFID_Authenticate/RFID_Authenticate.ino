//GeekChickens
//http://geekchickens.blogspot.com.es/
/*
SDA es el pin 10
SCK es el pin 13
MOSI es el pin 11
MISO es el pin 12
RST es el pin 9
*/
//-----------------------------------------------variables, includes, funciones, etc-------------------------------------------------
#include <SPI.h>
#include <MFRC522.h>
 
#define SS_PIN 10
#define RST_PIN 9
 
MFRC522 rfid(SS_PIN, RST_PIN);
 
int numero_serie[5]; // aqui guardaremos el número que nuestro sensor RFID detectará
 
 //-----------------------------------------------variables, includes,funciones, etc-------------------------------------------------
 
void setup()
{
  Serial.begin(9600);
  SPI.begin();
  rfid.init();
}
 
void loop()
{
   leer_usuario();
}
 
 
void leer_usuario()
{
    if (rfid.isCard())
    {
           if (rfid.readCardSerial())
            {
                Serial.print("Numero usuario: "); // guardamos el numero del usuario
                for(int i=0; i<=4 ; i++)
                {
                  numero_serie[i] = rfid.serNum[i];
                }
 
                for(int i=0; i<=4 ; i++) // y lo imprimimos
                {
                  Serial.print(numero_serie[i]);
                  Serial.print("    ");
                }
 
                Serial.println(" "); // espacio para el siguiente código
                delay(500); // para que no se imprima constantemente el mismo numero
             }
    }
    rfid.halt();
}
