/* 
  2 Tutorial sobre funcionamiento de lectura de 
  3 RFID modulo RC522 sobre Arduino Funduino.
  4 */
  5 
  6 #include 
  7 #include  
  8 
  9 #define SDA_I 10
 10 #define RST_I 9
 11 
 12 // Declaración de conexion de SDA y RST de RC522
 13 RFID rfid(SDA_I,RST_I); 
 14 
 15 int led_rojo = 8;
 16 int led_verde = 7;
 17 
 18 // Declaración de codigos / usuarios aceptados 
 19 int numero_serie[5];   
 20 int usuario_valido[1][5]={ {136,414,47,9,83} };
 21 
 22 int contador=0;
 23 int bandera=0;
 24 
 25 void setup(){
 26 // Iniciamos la configuracion     
 27 Serial.begin(9600);
 28 SPI.begin();
 29 rfid.init();
 30 pinMode(led_verde, OUTPUT);
 31 pinMode(led_rojo, OUTPUT);
 32 
 33 }
 34 
 35 // Loop principal de lectura del usuario
 36 
 37 void loop(){ 
 38 digitalWrite(led_verde,LOW);
 39 digitalWrite(led_verde,LOW);
 40 if (bandera==0){
 41         Serial.print("Esperando id......  ");
 42         Serial.println();
 43         delay(1000);
 44         bandera=1;
 45 }
 46 delay(100);    
 47 lectura_usuario();
 48 delay(100);
 49 }
 50 
 51 // Se detecta tarjeta compatible con el protocolo
 52 // y se imprime por pantalla el codigo
 53 
 54 void lectura_usuario(){
 55 contador = 0;
 56 if (rfid.isCard()){ 
 57     if (rfid.readCardSerial()){
 58         Serial.println("Usuario n:  ");
 59         for ( int i=0;i<=4;i++){
 60             numero_serie[i]=rfid.serNum[i];
 61         }
 62         for ( int j=0;j<=4; j++){
 63             Serial.print(numero_serie[j]);
 64         }
 65         delay(500);
 66         contador=1;
 67         comprobacion_id();
 68     }
 69     
 70 }
 71 rfid.halt();
 72 }
 73 
 74 // Se realiza la comparación entre los datos
 75 // leidos y los almacenados en la cadena "usuario_valido"
 76 
 77 
 78 void comprobacion_id(){
 79 int i,j=0;
 80         if ((usuario_valido[i][j])==(numero_serie[j])){
 81             j++;
 82         }
 83         else{
 84         i++;
 85         j=0;    
 86         }
 87         if(i >= 2){ 
 88 
 89                j=5;
 90          }
 91     switch(i)
 92     {
 93     case 0:
 94         usuario_validado();
 95         break;
 96         
 97     default:
 98         usuario_no_validado();
 99         break;
100         
101     }    
102 
103 bandera=0;
104 digitalWrite(led_verde,LOW);
105 digitalWrite(led_rojo,LOW);
106 }
107 
108 // Validacion de usuario mediante mensaje
109 // y encendido de Led Verde
110 
111 void usuario_validado(){
112     if(contador == 1)
113     {
114         Serial.print("  Usuario correcto");
115         Serial.println();
116         digitalWrite(led_verde,HIGH);
117         delay(1000);
118         contador = 0;
119     }
120 }
121 
122 // Negación de usuario mediante mensaje
123 // y encendido de Led Verde
124 
125 
126 void usuario_no_validado(){
127     if(contador == 1)
128     {
129         Serial.print("  Usuario incorrecto");
130         Serial.println();
131         digitalWrite(led_rojo,HIGH);
132         delay(1000);
133         contador = 0;
134     }
135 }
