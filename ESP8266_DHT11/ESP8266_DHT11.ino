//El ESP8266 lee datos del DHT11 y los envia al puerto serie.
//Compilar con la opcion Generic ESP8266 Module
// Libraries
#include "DHT.h"
// Pin
#define DHTPIN 2
// Use DHT11 sensor on GPIO2
#define DHTTYPE DHT11
// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE,15);
void setup() {
// Start Serial
Serial.begin(115200);
// Init DHT
dht.begin();
}
void loop() {
// Reading temperature and humidity
float h = dht.readHumidity();
// Read temperature as Celsius
float t = dht.readTemperature();
// Display data
Serial.print("Humidity: ");
Serial.print(h);
Serial.print(" %\t");
Serial.print("Temperature: ");
Serial.print(t);
Serial.println(" *C ");
// Wait a few seconds between measurements.
delay(2000);
}
