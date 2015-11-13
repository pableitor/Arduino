#include <SoftwareSerial.h>
SoftwareSerial BT1(9, 8); // D9 as RX conect to TX in other device | D2 as TX connect to RX other device

void setup()
  {  Serial.begin(115200);
     BT1.begin(115200);
  }

void loop()
  {  String B= "." ;
     if (BT1.available())
         { char c = BT1.read() ;
           Serial.print(c);
         }
     if (Serial.available())
         {  char c = Serial.read();
            BT1.print(c);
         }
   }
