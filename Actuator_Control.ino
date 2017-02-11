#include <ros.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Int8.h>
#include <math.h>

const int Adec = 4;
const int BDec = 5;
const int Cdec = 6;
const int Enable = 7;
const int Brake = 8;
const int Reset = 9;
const int Rec = 2; //this should be an interrupt
const int Ref = 10;
const int Ready = 11;
const int DecPow = 12; //not sure if necessary, may prevent partial write to decoder

int ActRef, ActPos;
float curpos;  
boolean reached;

void ActRefCB( const std_msgs::Int8& msg){
  ActRef = msg.data;
}
ros::Subscriber<std_msgs::Int8> ActRefSub("ActRef", ActRefCB);

void ActPosCB( const std_msgs::Float32& msg){
  ActPos = (int)round(1000*msg.data);
}
ros::Subscriber<std_msgs::Float32> ActPosSub("ActPos", ActPosCB);

void setup() {
  nh.initNode();

  nh.subscribe(ActPosSub);
  nh.subscribe(ActRefSub);

  pinMode(Adec, OUTPUT);
  pinMode(Bdec, OUTPUT);
  pinMode(Cdec, OUTPUT);
  pinMode(DecPow, OUTPUT);

  pinMode(Enable, OUTPUT);
  pinMode(Brake, OUTPUT);
  pinMode(Reset, OUTPUT);

  pinMode(Ref, INPUT);
  pinMode(Ready, INPUT);
  attachInterrupt(digitalPinToInterrupt(Rec), setReached, RISING);

  ActRef = 0;
  ActPos = 0;
  curpos = 0;
  reached = true;
  digitalWrite(Enable, HIGH);
}

void loop() {
  if(digitalRead(Ref) == LOW && ActRef == 1 && digitalRead(Ready) = HIGH){
    goHome();
  }

  if(ActPos != curpos && digitalRead(Ready) == HIGH && digitalRead(Ref) == HIGH){
    reached = false;
    moveToPos();
  }
  
}

void moveToPos(){
  while(reached = false){
    if(ActPos - curpos > 0){
      if(ActPos - curpos > 100){ //logarithmic moves in mm (100,10,1)
        digitalWrite(DecPow, LOW);//values go, 0-5 on dec
        digitalWrite(Adec, LOW);
        digitalWrite(Bdec, LOW);
        digitalWrite(Cdec, LOW);
        digitalWrite(DecPow, HIGH);        
      }if(Actpos - curpos > 10){
        digitalWrite(DecPow, LOW);
        digitalWrite(Adec, HIGH);
        digitalWrite(Bdec, LOW);
        digitalWrite(Cdec, LOW);
        digitalWrite(DecPow, HIGH);        
      }else{
        digitalWrite(DecPow, LOW);
        digitalWrite(Adec, LOW);
        digitalWrite(Bdec, HIGH);
        digitalWrite(Cdec, LOW);
        digitalWrite(DecPow, HIGH);        
      }
    }else if(ActPos - curpos < 0){
      if(ActPos - pos < -100){ //logarithmic moves in mm (100,10,1)
        digitalWrite(DecPow, LOW);
        digitalWrite(Adec, HIGH);
        digitalWrite(Bdec, HIGH);
        digitalWrite(Cdec, LOW);
        digitalWrite(DecPow, HIGH);        
      }if(Actpos - curpos < -10){
        digitalWrite(DecPow, LOW);
        digitalWrite(Adec, LOW);
        digitalWrite(Bdec, LOW);
        digitalWrite(Cdec, HIGH);
        digitalWrite(DecPow, HIGH);        
      }else{
        digitalWrite(DecPow, LOW);
        digitalWrite(Adec, HIGH);
        digitalWrite(Bdec, LOW);
        digitalWrite(Cdec, HIGH);
        digitalWrite(DecPow, HIGH);        
      }
    }else{
      return; //This should never happen, it means that we think we reached the goal, but we didn't
    }
    
  }

  
}


void goHome(){
  while(reached == false){
    digitalWrite(DecPow, LOW); //value 7 on dec
    digitalWrite(Adec, HIGH);
    digitalWrite(Bdec, HIGH);
    digitalWrite(Cdec, HIGH);
    digitalWrite(DecPow, HIGH);
  }
}

void setReached(){
  reached = true;
}
