// EmonLibrary examples openenergymonitor.org, Licence GNU GPL V3

#include "EmonLib.h"                   // Include Emon Library
EnergyMonitor emon1;                   // Create an instance

void setup()
{  
  Serial.begin(9600);
  
  emon1.current(1, 19.00);             // Current: input pin, calibration.
} 
/*El coeficiente de calibracion es el factor por el que hay que 
multiplicar la lectura en el sensor para obtener 1 V en el secundario
cuando tengamos 1A en el primario. En el caso del SCT013 cuando tenemos 100A
en el primario nos da 50mA , por lo que para 1 A en el primario tendriamos 
0.5 mA en el secundario.
Si ponemos una resistencia de carga de 100Ohm entonces el coeficiente 
sería INV(0.50mA*100)=20 . Lo hemos ajustado a mano comprobando la lectura
teorica con la de una pinza amperimetrica y nos da 19.12 en lugar de 20
*/
void loop()
{
  double Irms = emon1.calcIrms(1480)-0.13;  // Calculate Irms only
  // le restamos un offset de 0.13 para compensar el error de lectura a
  // corriente cero, debido a que el divisor  de tension de el circuito 
  //no da exactamente 2.5V
  
  Serial.print(Irms*230.0);	       // Apparent power
  Serial.print(" ");
  Serial.println(Irms);		       // Irms
}
