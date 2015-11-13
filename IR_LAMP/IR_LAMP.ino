/**
Questo programme permette di accendere una lampada, attraverso un relay, con un 
telecomando ad infrarossi. Questa versione non permette di modificare il codice
a run time
Autore Giacomo Bellazzi
UTILIZA LA TECLA ON/OFF DEL MANDO DINAVIN 
*/
 
#include <IRremote.h>
 
int RECV_PIN = 11;
int RELAY_PIN = 13;
unsigned long codice = 284151855; // codice DEC del telecomando
 
IRrecv irrecv(RECV_PIN);
decode_results results;
 
void setup()
{
  pinMode(RELAY_PIN, OUTPUT);
 
   Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
}
 
int on = 0;
unsigned long last = millis();
 
void loop() {
 
  if (irrecv.decode(&results)){
     if (millis() - last > 250 && results.value==codice ) {
      on = !on;
      digitalWrite(RELAY_PIN, on ? HIGH : LOW);
     }
     last = millis();      
     irrecv.resume(); 
  }
}
