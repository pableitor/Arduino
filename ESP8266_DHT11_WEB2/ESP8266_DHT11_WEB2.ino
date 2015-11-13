//ESP8266 lee DHT11 directamente y presenta los datos en una
//pagina web interna a la que incorpora datos externos leidos desde
//la web openweathermap.org

// Import required libraries
#include <ESP8266WiFi.h>
// WiFi parameters
const char* ssid = "ONO8A95";
const char* password = "pauladelavega5";

// Libraries
#include "DHT.h"
// Pin
#define DHTPIN 2
// Use DHT11 sensor
#define DHTTYPE DHT11
// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE, 15);
WiFiServer server(80);
void setup() {
// Start Serial
Serial.begin(115200);
// Init DHT
dht.begin();
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
} Serial.println("");
Serial.println("WiFi connected");
// Start the server
server.begin();
Serial.println("Server started");
// Print the IP address
Serial.println(WiFi.localIP());
}
void loop() {
  WiFiClient client = server.available();
// Reading temperature and humidity
float h = dht.readHumidity();
// Read temperature as Celsius
float t = dht.readTemperature();
String req = client.readStringUntil('\r');
Serial.println(req);
client.flush();

String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
s += "<head>";
s += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
s += "<meta http-equiv=\"refresh\" content=\"60\" />";
s += "<script src=\"https://code.jquery.com/jquery-2.1.3.min.js\"></script>";
s += "<link rel=\"stylesheet\" href=";
s += "\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.4/css/bootstrap.min.css\">";
s += "<style>body{font-size: 24px;} .voffset {margin-top: 30px;}</style>";
s += "</head>";
s += "<div class=\"container\">";
s += "<h1>WiFi Weather Station</h1>";
s += "<div class=\"row voffset\">";
s += "<div class=\"col-md-3\">Temperature: </div>";
s += "<div class=\"col-md-3\">" + String(t) + "</div>";
s += "<div class=\"col-md-3\">Humidity: </div>";
s += "<div class=\"col-md-3\">" + String(h) + "</div>";
s += "</div>";
s += "</div>";
s += "<div class=\"row voffset\">";
s += "<div class=\"col-md-3\">Exterior temperature: </div>";
s += "<div id=\"ext_temp\" class=\"col-md-3\"></div>";
s += "<div class=\"col-md-3\">Exterior humidity: </div>";
s += "<div id=\"ext_humidity\" class=\"col-md-3\"></div>";
s += "</div>";
s += "<script>$.ajax({url: ";
s += "\"http://api.openweathermap.org/data/2.5/weather?q=Madrid,es&appid=bd82977b86bf27fb59a04b61b657fb6f\",";
s += " crossDomain: true})";
s += ".done(function(result) {";
s += "$(\"#ext_temp\").html((result.main.temp - 273.15).toFixed(2)); ";
s += "$(\"#ext_humidity\").html(result.main.humidity);})";
s += "</script>";
client.print(s);
delay(1);
Serial.println("Client disconnected");

delay(2000);
}
