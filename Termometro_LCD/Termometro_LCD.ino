/*
  LiquidCrystal Library - Hello World

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch prints "Hello World!" to the LCD
 and shows the time.

  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystal
 */

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int tempReading; 
int voltReading; 
int voltPin=0; 
int tempPin = 1;        //the analog pin the TMP36's Vout (sense) pin is connected to
                        //the resolution is 10 mV / degree centigrade with a
                        //500 mV offset to allow for negative temperatures

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  analogReference(DEFAULT); //AREF = 5.0V
}

void loop() {
  tempReading = analogRead(tempPin); 
  voltReading= analogRead(voltPin); 
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 0);
 
  // converting that reading to voltage, for 3.3v arduino use 3.3

 
  String stringOne ="Temp: ";
  String stringTwo = " C";
  float temperatureC = (tempReading*5.0/1024)*100-50 ;  //converting from 10 mv per degree wit 500 mV offset
                                               //to degrees ((voltage - 500mV) times 100)
                                               // la lectura analogica tiene un fondo de escala de 3.9V
  lcd.print(stringOne + temperatureC + stringTwo );
  lcd.setCursor(0, 1);
  stringOne ="BAT : ";
  stringTwo = " V";
  float voltage=(voltReading*5.0/1024*23);  //Compensamos el divisor resistivo multiplicando por 3.2
                                               // la lectura analogica tiene un fondo de escala de 3.9V 
  lcd.print(stringOne + voltage + stringTwo );

}

