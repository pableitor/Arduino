//Envia datos del sensor DHT11 a Thingspeak SIN Arduino
//tenemos 3 sensores DHT11 que envian temp y humedad de 3 habitaciones al channel 61621 
// Este codigo es identico para cada uno de los ESP8266, sustituyendo el "fieldx" segun sea 
// el sensor 1 , 2 o 3 
//Sensor 1 :  TEMPERATURA->field 1 , HUMEDAD ->field 2
//Sensor 2 :  TEMPERATURA->field 3 , HUMEDAD ->field 4
//Sensor 3 :  TEMPERATURA->field 5 , HUMEDAD ->field 6

//descomentar segun el modulo que estemos programando

#define TEMPERATURA "&field1="
#define HUMEDAD "&field2="
//#define TEMPERATURA "&field3="
//#define HUMEDAD "&field4="
//#define TEMPERATURA "&field5="
//#define HUMEDAD "&field6="

//OJO!! SI sale error de lectura: ALGUNOS DHT11 tienen los pines de alimentacion y datos cambiados 
// WiFi parameters
#include <ESP8266WiFi.h>


//librerias del DHT 11
#include "DHT.h" 
#define DHTPIN 2 //pin de entrada de datos del DHT11 por GPIO2
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE, 15);

//-- Hardware Serial
#define _baudrate 115200

#define ledPin     0 // GPIO0 se enciende cuando se conecta a la red WIFI

//*-- IoT Information
#define SSID "TP-LINK_64EE"
#define PASS "25377019"
//#define SSID "ONO8A95"
//#define PASS "pauladelavega5"
//#define SSID "WLAN_AA2C"
//#define PASS "Calabasa11"
#define HOST "184.106.153.149" // ThingSpeak IP Address: 184.106.153.149

// GET /update?key=[THINGSPEAK_KEY]&field1=[data 1]&field2=[data 2]...;
String GET = "GET /update?key=M80R35SE7ZR0LU3X";

void setup() {
  dht.begin(); //Se inicia el sensor 
  Serial.begin( _baudrate );
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  WiFi.begin(SSID, PASS);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
    digitalWrite(ledPin,HIGH);
    Serial.println("WiFi connected");
}

void loop() {
   Serial.print("connecting to ");
   Serial.println(HOST);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(HOST, httpPort)) {
    Serial.println("connection failed");
    digitalWrite(ledPin, LOW);
    return;
  }
  digitalWrite(ledPin, HIGH); 
  float value_humid = dht.readHumidity(); //se lee la humedad
  float value_temp = dht.readTemperature(); 
  String temp =String(value_temp);// turn integer to string
  String humid=String(value_humid);// turn integer to string
//----- update the  Thingspeak string with 2 values
  String cmd = GET + TEMPERATURA + temp +HUMEDAD+ humid +"\r\n";
  client.print(cmd);
  Serial.println(cmd);//debug
  delay(60000); // envia un dato por hora
}




