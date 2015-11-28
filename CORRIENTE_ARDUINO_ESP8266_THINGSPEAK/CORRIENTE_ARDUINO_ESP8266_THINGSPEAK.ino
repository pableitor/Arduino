/* Lee datos del sensor de corriente y los envia a ThingSpeak 
 *  mediante WIFI del ESP8266
 */
#include <SoftwareSerial.h>
#include "EmonLib.h"                   // Include Emon Library
//*-- Hardware Serial
#define _baudrate 115200

//*-- IoT Information
#define SSID "ONO8A95"
#define PASS "pauladelavega5"
//#define SSID "WLAN_AA2C"
//#define PASS "Calabasa11"
#define IP "184.106.153.149" // ThingSpeak IP Address: 184.106.153.149
#define WIFI_OK 13
// GET /update?key=[THINGSPEAK_KEY]&field1=[data 1]&field2=[data 2]...;
String GET = "GET /update?key=A0KGYC1X7BXD4ST0";
EnergyMonitor emon1;  // Create an instance

void setup() {
  Serial.begin( _baudrate );
  digitalWrite(WIFI_OK,LOW);
  Serial.println("AT");
  delay(2000);
  if(Serial.find("OK"))
  {
    connectWiFi();
  }
  emon1.current(1, 19.5);// Current: input pin, calibration.
  /*El coeficiente de calibracion es el factor por el que hay que 
multiplicar la lectura en el sensor para obtener 1 V en el secundario
cuando tengamos 1A en el primario. En el caso del SCT013 cuando tenemos 100A
en el primario nos da 50mA , por lo que para 1 A en el primario tendriamos 
0.5 mA en el secundario.
Si ponemos una resistencia de carga de 100Ohm entonces el coeficiente 
sería INV(0.50mA*100)=20 . Lo hemos ajustado a mano comprobando la lectura
teorica con la de una pinza amperimetrica y nos da 19.12 en lugar de 20
*/
 emon1.voltage(2,209,1.7);
}

void loop() {
  emon1.calcVI(20,2000);          // Calculate all. No.of crossings, time-out
  String Vrms =String(emon1.Vrms);// turn integer to string
  String Irms =String(emon1.Irms);// turn integer to string
  String Wrms= String(emon1.realPower);// turn integer to string
  updateTS(Irms,Vrms,Wrms);
  delay(3000);
}
//----- update the  Thingspeak string
void updateTS( String I, String V, String W)
{
  // ESP8266 Client
  String cmd = "AT+CIPSTART=\"TCP\",\"";// Setup TCP connection
  cmd += IP;
  cmd += "\",80";
  Serial.println(cmd);
  delay(1000);
  if( Serial.find( "Error" ) )
  {
    return;
  }

  cmd = GET + "&field1=" + I +"&field2="+ V +"&field3="+ W +"\r\n";
  Serial.print( "AT+CIPSEND=" );
  Serial.println( cmd.length() );
  if(Serial.find( ">" ) )
  {
    Serial.print(cmd);
  }
  else
  {
      Serial.println( "AT+CIPCLOSE" );//close TCP connection
  }
}

boolean connectWiFi()
{
  Serial.println("AT+CWMODE=1");//WiFi STA mode - if '3' it is both client and AP
  delay(2000);
  //Connect to Router with AT+CWJAP="SSID","Password";
  // Check if connected with AT+CWJAP?
  String cmd="AT+CWJAP=\""; // Join accespoint
  cmd+=SSID;
  cmd+="\",\"";
  cmd+=PASS;
  cmd+="\"";
  Serial.println(cmd);
  delay(2000);
  if(Serial.find("OK"))
  {
    digitalWrite(WIFI_OK,HIGH);
    return true;
  }
  else
  {
    return false;
  }

  cmd = "AT+CIPMUX=0";// Set Single connection
  Serial.println( cmd );
  if( Serial.find( "Error") )
  {
    return false;
  }
}
