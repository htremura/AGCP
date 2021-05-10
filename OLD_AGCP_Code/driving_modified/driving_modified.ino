#include <avr/interrupt.h>  // Include interrupts library
unsigned int ovf;
void timerdel_init();
void timerdel_start();
void timerdel_stop();
void timerdel_print();
void timerdel_rst();

String input = "";
//steering
int Disable = 14;
int DIR = 15;
int STEP = 13;
//direction and accelleration
int Motor = 2;
int DirectionRelay = 3;
int ForwardSwitch = 5;
int ReverseSwitch = 4;
//turn sig
int LeftTurnSig = 6;
int RightTurnSig = 7;
//brake
int BrakeOn = 8;
int BrakeOnPWM = 9;
int BrakeOff = 10;
int BrakeOffPWM = 11;
int Brake = 0;
void setup() 
{
  // put your setup code here, to run once:
  pinMode(Disable, OUTPUT);
  pinMode(DIR, OUTPUT);
  pinMode(STEP, OUTPUT);
  
  pinMode(Motor,OUTPUT);
  pinMode(DirectionRelay,OUTPUT);
  pinMode(ForwardSwitch,INPUT);
  pinMode(ReverseSwitch,INPUT);
  
  pinMode(LeftTurnSig,OUTPUT);
  pinMode(RightTurnSig,OUTPUT);

  
  pinMode(BrakeOn,OUTPUT);
  pinMode(BrakeOnPWM,OUTPUT);
  pinMode(BrakeOff,OUTPUT);
  pinMode(BrakeOffPWM,OUTPUT);
  
  digitalWrite(Disable, LOW);
  digitalWrite(DIR, LOW);
  digitalWrite(STEP, LOW);
  
  digitalWrite(BrakeOn,LOW);
  digitalWrite(BrakeOff,LOW);
  analogWrite(BrakeOnPWM,240);
  analogWrite(BrakeOffPWM,240);
  
  digitalWrite(LeftTurnSig, HIGH);
  digitalWrite(RightTurnSig, HIGH);
  timerdel_init();
  
  Serial.begin(9600);
  
}

void loop() 
{
  int F_Direction = digitalRead(ForwardSwitch);
  int R_Direction = digitalRead(ReverseSwitch);
  int Pedal = analogRead(A0);
  
  while(F_Direction == HIGH || R_Direction == HIGH)
  {
    int F_Direction = digitalRead(ForwardSwitch);
    int R_Direction = digitalRead(ReverseSwitch);
    //Serial.println(F_Direction);
    //Serial.println(R_Direction);
    
    if(F_Direction == HIGH)
    {
      digitalWrite(DirectionRelay, LOW);
    }
    else if(R_Direction == HIGH)
    {
      digitalWrite(DirectionRelay,HIGH);
    }
    else
    {
      analogWrite(Motor,0);
      digitalWrite(DirectionRelay,LOW);
      break;
    }
//    Serial.println(Pedal);
    Pedal = analogRead(A0);
    
    if(Pedal < 80)
    {
      analogWrite(Motor,0);
    }
    else
    {
      int pwm = map(Pedal,80,885,0,255);
      if(R_Direction == HIGH)
      {
        analogWrite(Motor,pwm-5); 
      }
      else
      {
        analogWrite(Motor,pwm); 
      }
    }
    
  }
  while (Serial.available() > 0) 
  {
    char c = Serial.read();
    input += c;
    delay(5);
  }
  
  if(input != "")
  {
//    Serial.print("I received: ");
//    Serial.println(input);
    timerdel_start();
  }
  if(input.substring(0,1) == "S")
  {
    analogWrite(Motor,0);
    digitalWrite(DirectionRelay,LOW);
    timerdel_stop();
    timerdel_print();
    timerdel_rst();
  }
  
  if(input.substring(0,input.indexOf(":")) == "R")
  {
    digitalWrite(DirectionRelay,HIGH);
//    Serial.println("reverse");
    input.remove(0,(input.indexOf(":")+1));  
//    Serial.println(input);
    int pwm = map(input.toInt(),0,100,40,255);
    analogWrite(Motor,pwm);
//    Serial.println(pwm);
    timerdel_stop();
    timerdel_print();
    timerdel_rst();
    
  }
  else if(input.substring(0,input.indexOf(":")) == "F")
  {
    digitalWrite(DirectionRelay,LOW);
//    Serial.println("forward");
    input.remove(0,(input.indexOf(":")+1));
//    Serial.println(input);
    int pwm = map(input.toInt(),0,100,40,255);
    analogWrite(Motor,pwm);
//    Serial.println(pwm);
    timerdel_stop();
    timerdel_print();
    timerdel_rst();
    
  }
  else if(input.substring(0,input.indexOf(":")) == "TR")
  {
    input.remove(0,(input.indexOf(":")+2));
  }
  else if(input.substring(0,input.indexOf(":")) == "TL")
  {
    input.remove(0,(input.indexOf(":")+2));
  }
  else if(input.substring(0,input.indexOf(":")) == "B")
  {
//    Serial.println("Brake");
    input.remove(0,(input.indexOf(":")+1));
//    Serial.println(input);
    
    if(input.toInt() == 1)
    {
      delay(500);
//      Serial.println("on");
      digitalWrite(BrakeOff,LOW);
      delay(500);
      digitalWrite(BrakeOn,HIGH);
    }
    else
    {
      delay(500);
//      Serial.println("off");
      digitalWrite(BrakeOn,LOW);
      delay(500);
      digitalWrite(BrakeOff,HIGH);
    }    

    delay(2000);
    digitalWrite(BrakeOn,LOW);
    digitalWrite(BrakeOff,LOW);
  }
  else if(input.substring(0,2) == "LL")
  {
//    Serial.println("Left");
    digitalWrite(LeftTurnSig,LOW);
    digitalWrite(RightTurnSig,HIGH);
  }
  else if(input.substring(0,2) == "RL")
  {
//    Serial.println("Right");
    digitalWrite(RightTurnSig,LOW);
    digitalWrite(LeftTurnSig,HIGH);
  }
  else if(input.substring(0,2) == "FL")
  {
//    Serial.println("Flash");
    digitalWrite(RightTurnSig,LOW);
    digitalWrite(LeftTurnSig,LOW);
  }
  else if(input.substring(0,2) == "SL")
  {
//    Serial.println("Flash off");
    digitalWrite(RightTurnSig,HIGH);
    digitalWrite(LeftTurnSig,HIGH);
  }

  
  input = "";
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
  Serial.print(TCNT2 + (256*ovf));
}
void timerdel_rst(){
  ovf = 0;
  TCNT2 = 0;
}
