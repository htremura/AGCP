#include <avr/interrupt.h>  // Include interrupts library
unsigned int ovf;
void timerdel_init();
void timerdel_start();
void timerdel_stop();
void timerdel_print();
void timerdel_rst();

String input = "";                                                  // Empty Input
int DIS = 6;                                                        // Disable (MOTOR POWER)
int DIR = 7;                                                        // Direction
int STEP = 11;                                                      // Pin that controls steps of motor
int NUM = 0;                                                        // Defining names for pins and number of steps to take

void setup()          //
{
  TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);     //
  TCCR2B = _BV(CS00)|_BV(CS01);                                     //
  OCR2A = 180;                                                      //
  OCR2B = 50;                                                       // Increasing the PWM frquency
  timerdel_init();
  pinMode(STEP, OUTPUT);                                            // set pin directions
  pinMode(DIR,OUTPUT);                                              // set pin directions
  pinMode(DIS,OUTPUT);                                              // set pin directions
  digitalWrite(DIS,HIGH);                                           // Disabling stepper motor
  Serial.begin(9600);                                               // Begin Serial Comm
}

void loop() {
  // put your main code here, to run repeatedly:
  while (Serial.available() > 0)                                    // Reading the serial input and placing it into "input"
  {                                                                 //
    char c = Serial.read();                                         //
    input += c;                                                     //
    delay(5);                                                       //
  }

  if(input != "")                                                   // Echo message
  {                                                                 //
    timerdel_start();
//    Serial.println("I received: ");                                   //
//    Serial.println(input);                                          //
  }                                                                 //
  
  if(input.substring(0,input.indexOf(":")) == "TR")                 // If TR is in the string enter this function
  {                                                                 //
    input.remove(0,(input.indexOf(":")+1));                         // Remove TR
//    Serial.println(input);                                          // Print number of steps
    NUM = input.toInt();                                            // Convert input to int
    if(NUM == 0)                                                    // if input is 0
    {                                                               // Keep turning right
//      Serial.println("Right");                                      //
      digitalWrite(DIR, LOW);                                       //
      digitalWrite(DIS,LOW);                                        //
      analogWrite(STEP,200);                                        //
      timerdel_stop();
      timerdel_print();
      timerdel_rst();
    }                                                               
    else                                                            // Otherwise
    {                                                               // Turn right a specified number of steps
//      Serial.println("Right1");                                       //
      digitalWrite(DIS,LOW);                                        //
      digitalWrite(DIR, LOW);                                       //
      timerdel_stop();
      timerdel_print();
      timerdel_rst();
      for(int i = 0; i<NUM;i++)                                     //
      {                                                             //
       digitalWrite(STEP,HIGH);                                     //
       delay(1);                                                    //
       digitalWrite(STEP,LOW);                                      //
       delay(1);                                                    //
      }
    }
  }
  else if(input.substring(0,input.indexOf(":")) == "TL")            // Same as TR
  {
    input.remove(0,(input.indexOf(":")+1));
//    Serial.println(input);
    NUM = input.toInt();
    if(NUM == 0)
    {
//      Serial.println("Left");
      digitalWrite(DIR, HIGH);
      digitalWrite(DIS,LOW);
      analogWrite(STEP,200);
      timerdel_stop();
      timerdel_print();
      timerdel_rst();
    }
    else
    {
//      Serial.println("Left1");
      digitalWrite(DIS,LOW);
      digitalWrite(DIR, HIGH);
      timerdel_stop();
      timerdel_print();
      timerdel_rst();
      for(int i = 0; i<NUM;i++)
      {
       digitalWrite(STEP,HIGH);
       delay(1);
       digitalWrite(STEP,LOW);
       delay(1);
      }
    }
    
  }
  if(input.substring(0,1) == "S")
  {
//    Serial.println(input);
//    Serial.println("STOP");
    digitalWrite(DIS,HIGH);
    timerdel_stop();
    timerdel_print();
    timerdel_rst();
  }
  input = "";

  
  //digitalWrite(DIR, LOW);
  //digitalWrite(DIS,LOW);
  //analogWrite(STEP,200);

  // SUMMARY
  // IF MESSAGE BEGINS WITH TR OR TL, TURN, A SPECIFIED NUMBER OF STEPS OR FOREVER
}
ISR(TIMER2_OVF_vect) {
  ovf++;
}
void timerdel_init(){
  ovf = 0;
  TCNT2 = 0;
  TIMSK2 |= (1 << TOIE2);
}
void timerdel_start(){
  TCCR2B |= (1 << CS20);
}
void timerdel_stop(){
  TCCR2B &= 0;
}
void timerdel_print(){
  Serial.println(TCNT2 + (256*ovf));
}
void timerdel_rst(){
  ovf = 0;
  TCNT2 = 0;
}
