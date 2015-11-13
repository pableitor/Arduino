/*
/*
** Rotary Encoder Example
** Use the Rotary Encoder to output ASCII chars
**
** Sample the encoder at 200Hz using the millis() function
*/

int ascii = 32;    // start at ascii 0
int incr = 1;    // how many points to increment per reading
unsigned long currentTime;
unsigned long loopTime;
const int pin_A = 12;  // pin 12
const int pin_B = 11;  // pin 11
unsigned char encoder_A;
unsigned char encoder_B;
unsigned char encoder_A_prev=0;
boolean change=false;

void setup()  {
  Serial.begin(9600);
  pinMode(pin_A, INPUT);
  pinMode(pin_B, INPUT);
  currentTime = millis();
  loopTime = currentTime; 
} 

void loop()  {
  // get the current elapsed time
  currentTime = millis();
  if(currentTime >= (loopTime + 3)){ //3ms. per reading
    // 1ms since last check of encoder = 1000Hz  
    encoder_A = digitalRead(pin_A);    // Read encoder pins
    encoder_B = digitalRead(pin_B);   
    if((!encoder_A) && (encoder_A_prev)){
      // A has gone from high to low 
      change=true;
      if(encoder_B) {
        // B is high so clockwise
        // increase the code, if over 126 rollback to 32
        if(ascii + incr <= 126) ascii += incr;
          else ascii=32;               
      }   
      else {
        // B is low so counter-clockwise      
        // decrease the code, if below 32 (space) roll to 126
        if(ascii - incr >= 32) ascii -= incr;
          else ascii=126;               
      } 
    }
    else if((encoder_A) && (!encoder_A_prev)){
    // A has gone from low to high 
      change=true;
      if(!encoder_B) {
        // B is low so clockwise
        // increase the code, if over 126 rollback to 32
        if(ascii + incr <= 126) ascii += incr;
          else ascii=32;               
      }   
      else {
        // B is high so counter-clockwise      
        // decrease the code, if below 32 (space) roll to 126
        if(ascii - incr >= 32) ascii -= incr;
          else ascii=126;               
      } 
    } 
    if (change){
      //Print ascii char
      Serial.write(ascii);   
      Serial.print(", dec: ");
      Serial.print(ascii); 
      Serial.print(", hex: ");
      Serial.println(ascii,HEX);    
      change=false;
    }

    loopTime = currentTime;  // Updates loopTime    
    
  }   
  encoder_A_prev = encoder_A;     // Store value of A for next time    
   
  // Other processing can be done here
                           
}

