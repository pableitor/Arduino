// Import required libraries
#include <ESP8266WiFi.h>
void setup(void)
{
// Start Serial
Serial.begin(115200);
// Set GPIO 0 as input
pinMode(2, INPUT);
}
void loop() {
// Read GPIO 5 and print it on Serial port
Serial.print("State of GPIO 2: ");
Serial.println(digitalRead(2));
// Wait 1 second
delay(1000);
}

