// Copyright (C) 2014 SISTEMAS O.R.P.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
 
#define SEARCH_AFTER_RESET "ready"
#define INTRO "\r\n"
 
#define AP_NORMAL "TP-LINK_64EE"
#define PWD_NORMAL "25377019"
#define HOST_NORMAL "mundocaraudio.dyndns.org" //ip del PC host puede ser una direccion de internet o IP local
#define PORT_NORMAL "49000"
 
#define AP_BOOTLOADER "TP-LINK_64EE"
#define PWD_BOOTLOADER "25377019"
#define HOST_BOOTLOADER "mundocaraudio.dyndns.org" //ip del host
#define PORT_BOOTLOADER "50000"
#define ESP_RES 10
#define UPDATE_FILE "Arduino_remote_example.cpp.hex"
#define MESSAGE "Version 3.6"
 
boolean ok = false;
int counter = 0;

//-----------------------------------------------------------------------------------------------------------------------------------
/*
Parameters:
  command: The string to send.
  timeout: The maximum time in milliseconds the function can be running before a time out.
  wait_for1: The search string when the command succeeded.
  wait_for1: The search string when the command failed.
Returns: 
  The string contained in wait_for1, wait_for2 or the string TIMEOUT.
Description: 
  It sends the command trough the serial port and waits for one of the expected strings or exit with timeout
*/
String send(String command, int timeout, String wait_for1, String wait_for2)
{
  unsigned long time = millis();
  String received = "";
  
  Serial.print(command);
  Serial.print(INTRO);
  
  while(millis() < (time + timeout))
  {
    if(Serial.available() > 0)
    {
      received.concat(char(Serial.read()));
      if(received.indexOf(wait_for1) > -1)
      {
        return wait_for1;
      }
      else if(received.indexOf(wait_for2) > -1)
      {
        return wait_for2;
      }
    }
  }
  
  return "TIMEOUT";
}
//-----------------------------------------------------------------------------------------------------------------------------------
/*
Parameters:
  wait_for: The search string.
  timeout: The maximum time in milliseconds the function can be running before a time out.
Returns: 
  True if the string was found, otherwise False.
Description: 
  It waits for the expected string.
*/
boolean look_for(String wait_for, int timeout)
{
  unsigned long time = millis();
  String received = "";
  
  while(millis() < (time + timeout))
  {
    if(Serial.available() > 0)
    {
      received.concat(Serial.readString());
      if(received.indexOf(wait_for) > -1)
      {
        return true;
      }
    }
  }
  
  return false;
}
//-----------------------------------------------------------------------------------------------------------------------------------
/*
Parameters:
  command: The string to send.
  timeout: The maximum time in milliseconds the function can be running before a timeout.
  wait_for1: The search string when the command succeeded.
  wait_for1: The search string when the command failed.
Returns: 
  True if the wait_for1 string was found, otherwise False.
Description: 
  It sends the command trough the serial port and waits for one of the expected strings or exit with timeout.
*/
boolean send_command(String command, int timeout, String wait_for1, String wait_for2)
{
  String state;
  
  state = send(command, timeout, wait_for1, wait_for2);
  if(state == wait_for1)
  {
    return true;
  }
  else if(state == wait_for2)
  {
    // do something on error
    Serial.println("Error enviando "+command+" Recibido "+wait_for2);

  }
  else
  {
        // do something on timeout
        Serial.println("Timeout enviando "+command);

  }
  
  return false;
}
//-----------------------------------------------------------------------------------------------------------------------------------
/*
Parameters:
  Nothing
Returns: 
  True if all commands were successfully, otherwise False.
Description: 
  It initializes the module, joins to the access point and connects to the server.
*/
boolean init_commands()
{
  // print_lcd("Changing mode");

    pinMode(ESP_RES,OUTPUT); //envia pulso de reset al ESP8266
    digitalWrite(ESP_RES,LOW);
    delay(100);
    digitalWrite(ESP_RES,HIGH);
    delay(2000);
    while(Serial.available() > 0) Serial.read(); //limpia buffer de basura del ESP8266 tras reset

  if(send_command("AT+CWMODE=1", 5000, "OK", "ERROR"))

  {
    
    if (send_command("AT+RST", 5000, SEARCH_AFTER_RESET, "ERROR"))
    {

  
      String cwjap = "AT+CWJAP=\"";
      cwjap += AP_NORMAL;
      cwjap += "\",\"";
      cwjap += PWD_NORMAL;
      cwjap += "\"";
      if (send_command(cwjap, 20000, "OK", "FAIL"))
        if (send_command("AT+CIPMUX=0", 2000, "OK", "ERROR"))
          if (send_command("AT+CIPMODE=0", 2000, "OK", "ERROR"))
          {
            // print_lcd("Connecting host");
            
            String cipstart = "AT+CIPSTART=\"TCP\",\"";
            cipstart += HOST_NORMAL;
            cipstart += "\",";
            cipstart += PORT_NORMAL;
            if (send_command(cipstart, 5000, "OK", "ERROR"))
              return true;
          }
    }
  }
            
  return false;
}
//-----------------------------------------------------------------------------------------------------------------------------------
/*
Parameters:
  Nothing
Returns: 
  True if all commands were successfully, otherwise False.
Description: 
  It initializes the module, joins to the access point, connects to the server and starts the protocol.
*/
boolean boot_commands()
{
  
  String cwjap = "AT+CWJAP=\"";
  cwjap += AP_BOOTLOADER;
  cwjap += "\",\"";
  cwjap += PWD_BOOTLOADER;
  cwjap += "\"";
  if (send_command(cwjap, 20000, "OK", "FAIL"))
    if (send_command("AT+CIPMUX=0", 2000, "OK", "ERROR"))
      if (send_command("AT+CIPMODE=1", 2000, "OK", "ERROR"))
      {
        
        String cipstart = "AT+CIPSTART=\"TCP\",\"";
        cipstart += HOST_BOOTLOADER;
        cipstart += "\",";
        cipstart += PORT_BOOTLOADER;
        if (send_command(cipstart, 5000, "OK", "ERROR"))
          if (send_command("AT+CIPSEND", 2000, ">", "ERROR"))
          {

            if (send_command("hello", 2000, "welcome", "error"))
              if (send_command(UPDATE_FILE, 2000, "ok", "error"))
                return true;
          }
      }
            
  return false;
}
//-----------------------------------------------------------------------------------------------------------------------------------
/*
Parameters:
  Nothing
Returns: 
  True if all commands were successfully, otherwise False.
Description: 
  It sends a string to the remote host and show it in the display.
*/
boolean test()
{
  String command = "AT+CIPSEND=";
  String to_send = MESSAGE;
  to_send += counter;
  command += to_send.length() + 2;
  
  if (send_command(command, 2000, ">", "ERROR"))
    if (send_command(to_send + "\r\n", 2000, "OK", "ERROR"))
    {

      counter++;
      return true;
    }
  
  return false;
}
//-----------------------------------------------------------------------------------------------------------------------------------
void setup()
{
  pinMode(13, OUTPUT);

  Serial.begin(115200);

  delay(3000);
  while(Serial.available() > 0) Serial.read();
  
  ok = init_commands();
}
//-----------------------------------------------------------------------------------------------------------------------------------
void loop()
{
  if(ok)
  {
    digitalWrite(13, HIGH);// Conexion correcta
    ok = test(); //Envia mensajes al servidor 
    if(ok && look_for("reboot", 1000))
    {  //chequea continuamente si recibido comando reboot para actualizar
      if(boot_commands()) //detectada orden de reboot
      {
        Serial.write(00); //envia 00 al servidor para que envie el codigo
        
        pinMode(12, OUTPUT); //una vez terminada la reprogramacion realiza un reset
        digitalWrite(12, LOW);//Arduino se resetea a si mismo (pin 12 conectado a RESET)
        for(;;);
      }
      else
      {
        ok = false;
      }
    }
  }  
  else
  {
    digitalWrite(13, LOW); // led 13 apagado indica error

    ok = init_commands();
  }
}
