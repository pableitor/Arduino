//Envia datos del sensor DHT11 a Thingspeak SIN Arduino

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
#define SSID "ONO8A95"
#define PASS "pauladelavega5"
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
    return;
  }
   
   float value_humid = dht.readHumidity(); //se lee la humedad
   float value_temp = dht.readTemperature(); 
  String temp =String(value_temp);// turn integer to string
  String humid=String(value_humid);// turn integer to string
//----- update the  Thingspeak string with 2 values
  String cmd = GET + "&field1=" + temp +"&field2="+ humid +"\r\n";
  client.print(cmd);
  delay(3000); //
}




