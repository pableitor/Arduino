#include <Ethernet.h>
#include <SoftwareSerial.h>
#include <EEPROM.h>
 
#define DEBUG 1               // set to 1 to display debug info via seral link
 
int ledalarm = 3;               // pin the LED is on   (you can't use the onboard one as the ethernet card uses it)
int pulsalarm = 9;
int pulspause = 8;
int alarmout = 4;
int speakerPin = 5;
 
char TextBox1[10];            // Data in text box
 
int length = 15; // the number of notes
char notes[] = "ccggaagffeeddc "; // a space represents a rest
int beats[] = { 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 4 };
int tempo = 300;
boolean alarm=0;
 
#define APIKEY  "**************"
#define emonlink "http://emoncms.org/boris&id=2505"
 
// ethernet interface mac address
static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };
// ethernet interface ip address
static byte myip[] = { 192,168,0,99 };
// gateway ip address
static byte gwip[] = { 192,168,0,1 };
 
static const byte dnsip[] = { 192,168,0,1 };
 
char website[] PROGMEM = "emoncms.org";
static const byte hisip[] = { 213,138,101,177 };
 
unsigned long previousMillis_power=0;
int power_postrate=10000;   //post every 10s 
int watts=0;
int maxwatts=3000;
byte inData1[10]; // Allocate some space for the string
byte inData2[10];
//byte inData3[10];
 
boolean pause=0;
 
char str[20];  
 
SoftwareSerial mySerial(6, 7); // RX, TX
 
// web page buffer
  byte Ethernet::buffer[800];
  BufferFiller bfill;
 
// store html header in flash to save memory
char htmlHeader[] PROGMEM = 
      "<html><head><title>Power meter by Boris</title></head><body>"
      "<h2 style='text-align: center;'><em><span style='color: rgb(153, 0, 0);'>Power meter by Boris</span></em></h2>"
;
 
// ----------------------------------------------
 
// HTML page to display
 
static word homePage() {
    checkmcee();
    bfill = ether.tcpOffset();
 
    // read A0 status
    //word sensorValue = analogRead(sensorPin);    
 
    maxwatts =  EEPROM_readint(0);
    Serial.print("maxwatts in mem : ");
    Serial.println(maxwatts);
 
    Serial.print("alarm in mem : ");
    Serial.println(EEPROM_readint(5));
 
    // read statues of the LED
    char* alarmstat;
    if ( EEPROM_readint(5) == 1 ) {
      alarmstat = "On" ; }
    else {
      alarmstat = "Off"; }
 
    bfill.emit_p( PSTR ( 
      "$F<p>"    // $F = htmlheader in flash memory
      "<center>"
      "<iframe src='$S' width='1000' height='500' frameborder='0' ></iframe><br/>"
      "<em>Alarm: $S <br/>"
      "Alarm threshold: $D <br/>"
      "</em></p><p>"
      "<A HREF='?cmd=on'>Alarm on</A><br/><br/>"
      "<A HREF='?cmd=off'>Alarm off</A><br/><br/><br/>"
      "<A HREF='?cmd=pause'>Alarm pause</A><br/><br/>"
      "<FORM>Insert alarm threshold <input type=text name=maxwatts size=4 value=$D> <input type=submit value=Enter> </form> <br/><br/>"
      "<A HREF='http://www.open-electronics.org'>More info</A>"
      "</p></center></body></html>"
    ) , htmlHeader , emonlink, alarmstat , maxwatts , maxwatts ) ;
 
    return bfill.position(); 
 
  }
 
// ----------------------------------------------
 
void setup () {
 
  pinMode(ledalarm, OUTPUT);  
  pinMode(alarmout, OUTPUT);  
  pinMode(speakerPin, OUTPUT);
  pinMode(pulsalarm, INPUT);
  digitalWrite(pulsalarm, HIGH);
  pinMode(pulspause, INPUT);
  digitalWrite(pulspause, HIGH);
 
  Serial.begin(9600);
  Serial.println("\n[webClient]");
 
  if ((EEPROM.read(10)!=10)){
    Serial.println("First power on");  
    EEPROM.write(10, 10);
    EEPROM_writeint(0, maxwatts);    
    alarmoff();
  }
  else
  {
    Serial.println("Data on eeprom already stored");  
  }
 
    maxwatts =  EEPROM_readint(0);
    Serial.print("maxwatts in mem : ");
    Serial.println(maxwatts);
 
    Serial.print("alarm in mem : ");
    Serial.println(EEPROM_readint(5));
    if (EEPROM_readint(5)==1){
      digitalWrite(ledalarm, HIGH);
    }
 
  if (!ether.begin(sizeof Ethernet::buffer, mymac,10))
    Serial.println( "Failed to access Ethernet controller");
 else
   Serial.println("Ethernet controller initialized");
 
  ether.staticSetup(myip,gwip,dnsip);
  ether.copyIp(ether.hisip, hisip);
 
  ether.printIp("IP Address:\t", ether.myip);
  ether.printIp("Netmask:\t", ether.mymask);
  ether.printIp("Gateway:\t", ether.gwip);
  ether.printIp("DNS:\t", ether.dnsip);  
  ether.printIp("SRV: ", ether.hisip);  
 
  mySerial.begin(9600);
 
}
 
// ----------------------------------------------
 
void loop () {
 
  word len = ether.packetReceive();
  word pos = ether.packetLoop(len);
  if(pos) {
        Serial.print("pos ");
        Serial.println(pos);
        printpage(pos);
  }
 
if ((millis()%1000)==0){
  Serial.println(millis());
  delay(50);
    if (pause==1)
    {
      digitalWrite(ledalarm, HIGH);
      delay(200);
      digitalWrite(ledalarm, LOW);
      delay(200);  
    }
    else
    {
      if (alarm=1)
      {
        digitalWrite(ledalarm, HIGH);
      }
      else
      {
        digitalWrite(ledalarm, LOW);
      }
    }
}  
 
if ((millis() - previousMillis_power) > (power_postrate) )
  {
    previousMillis_power=millis(); 
 
    checkmcee();
 
    Serial.print("watts ");
    Serial.println (watts,DEC);
 
    pubblica();
    checkalarm();    
 
  }
 
  if (digitalRead(pulsalarm)==0){  //toggle alarm
 
    if (EEPROM_readint(5)==1) {
      alarmoff(); 
      delay(500);     
    }
    else
    {
      alarmon();  
      delay(500);
    }   
  } 
 
  if (digitalRead(pulspause)==0){  //toggle alarm
    pause=1; 
    Serial.println("pause on");  
  } 
 
}
 
void alarmon(){
  digitalWrite(ledalarm, HIGH);   // set the LED on
  EEPROM_writeint(5, 1);   //alarm off 
  Serial.println("alarm on");
  alarm=1;
}
 
void alarmoff(){
  digitalWrite(ledalarm, LOW);   // set the LED on
  EEPROM_writeint(5, 0);   //alarm off 
  Serial.println("alarm off"); 
  alarm=0;
}
 
void printpage(word pos){
 
        char* bufferdata = (char *) Ethernet::buffer + pos;
 
         Serial.println("----------------");
         Serial.println("data received:");
         Serial.println(bufferdata);
         Serial.println("----------------");
 
       // "on" command received     
       if (strncmp( "GET /?cmd=on" , bufferdata , 12 ) == 0) {
           alarmon();
       }
 
       // "off" command received     
       if (strncmp( "GET /?cmd=off" , bufferdata , 13 ) == 0) {
           alarmoff();
       }
 
       // "pause" command received     
       if (strncmp( "GET /?cmd=pause" , bufferdata , 13 ) == 0) {
           //digitalWrite(LEDpin, LOW);   // set the LED on
           Serial.println("pause received");
           pause=1;
       }
 
       // read data from text box
       if (strncmp( "GET /?maxwatts=" , bufferdata , 11 ) == 0) {
           Serial.print ( "text box input received - " );
           if (ether.findKeyVal(bufferdata + 6, TextBox1 , sizeof TextBox1 , "maxwatts") > 0) {
 
             Serial.print ( "input = " );
             Serial.println ( TextBox1 );
             EEPROM_writeint(0, atoi(TextBox1));
 
           }
       }
 
      ether.httpServerReply(homePage()); // send web page data
 
}
 
void checkmcee(){
    Serial.println("checkmcee ");
    int data=0;
    int index=0;
    mySerial.write((byte)0xAA);
    mySerial.write((byte)0x02);
    mySerial.write((byte)0x00);
    mySerial.write((byte)0xAD);
    delay(500);
    while (mySerial.available() > 0) {
      byte inByte = mySerial.read();
      //Serial.write(inByte);
      if (inByte=='S'){
        index=0;
        data++;
      }
 
      switch (data){
       case 1: 
         inData1[index] = inByte; // Store it
         index++;
         break;
       case 2:
         inData2[index] = inByte; // Store it
         index++;
         break;
      }
 
    }
 
    watts=0;
    watts=(inData2[6]<<8) + inData2[5];
 
}
 
void pubblica()
{
    str[0]='\0';
    srtJSON(str);                                   //Start JSON
    addJSON(str,"realP",watts);                   //JSON line 1 - add more lines as needed 
    endJSON(str);
    Serial.println(str);
    Serial.println(strlen(str));
 
    ether.browseUrl(PSTR("/api/post?apikey="APIKEY"&json="), str, website, my_result_cb); 
 
}
 
void checkalarm(){
 
if (alarm=1){  
  if (watts>maxwatts){
    if (pause==0){
      if (EEPROM_readint(5)==1)    //if alarm on
      {  
          digitalWrite(alarmout, HIGH);
          play();        
      }
    }        
  }
  else{
    pause=0;
    digitalWrite(alarmout, LOW);
  }
}  
}
 
// called when the client request is complete
static void my_result_cb (byte status, word off, word len) {
  Serial.print("<<< reply ");
  //Serial.print(millis() - timer);
  //Serial.println(" ms");
  Serial.println((const char*) Ethernet::buffer + off);
}
 
//write word to EEPROM
void EEPROM_writeint(int address, int value) {
  EEPROM.write(address,highByte(value));
  EEPROM.write(address+1 ,lowByte(value));
}
 
unsigned int EEPROM_readint(int address) {
  unsigned int word = word(EEPROM.read(address), EEPROM.read(address+1));
  return word;
} 
 
void playTone(int tone, int duration) {
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(speakerPin, HIGH);
    delayMicroseconds(tone);
    digitalWrite(speakerPin, LOW);
    delayMicroseconds(tone);
  }
}
 
void playNote(char note, int duration) {
  char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
  int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };
 
  // play the tone corresponding to the note name
  for (int i = 0; i < 8; i++) {
    if (names[i] == note) {
      playTone(tones[i], duration);
    }
  }
}
 
void play(){
  for (int i = 0; i < length; i++) {
    if (notes[i] == ' ') {
      delay(beats[i] * tempo); // rest
    } else {
      playNote(notes[i], beats[i] * tempo);      
    }
 
    // pause between notes
    delay(tempo / 2); 
    if ((digitalRead(pulsalarm)==0)||(digitalRead(pulspause)==0)){
      break;
    }
  }
}
