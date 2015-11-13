#include <Ethernet.h>
 
//Se introducen los valores correspondientes a MAC, IP local, Puerta de Enlace y Máscara de Red
byte mac[] = { 
  0x90, 0xA2, 0xDA, 0x0D, 0xF3, 0xA3 };
  
IPAddress ip(192,168,1,100);
 
IPAddress gateway(192, 168, 1, 1);
 
IPAddress subnet(255, 255, 255, 0);
 
//Se inicializa la librería Ethernet con el Puerto que se va utilizar en la transmisión
EthernetServer server(80);
 
void setup() {
 // Se establece comunicación con el monitor serial (para ver si el sistema está funcionando correctamente)
  Serial.begin(9600);
 
//   while (!Serial) {
//    ; // Se espera a que se conecte el puerto serie (solo para Arduino Leonardo)
//  }
 
 
  // Comienzo de la conexión
  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();
  //Se muestra por el monitor serial que la conexión está establecida
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}
 
 
void loop() {
  // Se espera a que alguien acceda a la página web
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // una petición http termina con una línea en blanco
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        //Si se llega al final de la línea se recive un nuevo carácter de
        //línea y si se trata de una línea en blanco la petición http ha terminado
        //así que se envía la respuesta 
        if (c == '\n' && currentLineIsBlank) {
          // cabezera típica http
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // se cierra la conexión una vez se ha respondido a la petición
   client.println("Refresh: 5");  // se refresca la página automáticamente cada 5 segundos
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          
          // se muestran en la web los valores de las entradas analógicas
          for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
            int sensorReading = analogRead(analogChannel);
            client.print("analog input ");
            client.print(analogChannel);
            client.print(" is ");
            client.print(sensorReading);
            client.println("<br />");       
          }
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // se comienza una nueva línea
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // se ha obtenido un carácter en la línea actual
          currentLineIsBlank = false;
        }
      }
    }
    // se le da tiempo al navegador para recibir los datos
    delay(1);
    // se cierra la conexión
    client.stop();
    Serial.println("client disonnected");
  }
}
