void setup() {
  // put your setup code here, to run once:
        pinMode(10, OUTPUT);
        digitalWrite(10, LOW);
        delay(500);
        digitalWrite(10,HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
        digitalWrite(10, LOW);
        delay(500);
        digitalWrite(10,HIGH);
        delay(2000);
}
