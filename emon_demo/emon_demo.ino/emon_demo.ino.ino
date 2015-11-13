#include "EmonLib.h"              // Include Emon Library
EnergyMonitor emon1;              // Create an instance

void setup()
{  
  Serial.begin(9600);
  
  emon1.voltage(2, 216, 1.7);  // Voltage: input pin, calibration, phase_shift
  emon1.current(1, 19);        // Current: input pin, calibration.
}

void loop()
{
  emon1.calcVI(20,2000);          // Calculate all. No.of crossings, time-out
  //emon1.serialprint();            // Print out all variables
  Serial.print("realPower       =");
  Serial.println( emon1.realPower);        //extract Real Power into variable
  Serial.print("apparentPower   =");
  Serial.println( emon1.apparentPower);    //extract Apparent Power into variable
  Serial.print("powerFActor     =");
  Serial.println( emon1.powerFactor);      //extract Power Factor into Variable
  Serial.print("supplyVoltage   =");
  Serial.println(emon1.Vrms);             //extract Vrms into Variable
  Serial.print("Irms            =");
  Serial.println(emon1.Irms);             //extract Irms into Variable
   
}

