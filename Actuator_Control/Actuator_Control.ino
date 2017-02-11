#include <ros.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Int8.h>
#include <math.h>

ros::NodeHandle nh;


const int Adec = 4;
const int Bdec = 5;
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
boolean reached, refbool;

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
  attachInterrupt(0, setReached, RISING);

  ActRef = 0;
  ActPos = 0;
  curpos = 0;
  reached = true;
  digitalWrite(Enable, HIGH);
  refbool = false;
}

void loop() {
  if(digitalRead(Ref) == LOW && ActRef == 1){// && digitalRead(Ready) == HIGH){
    reached = false;
    goHome();
    curpos = 0;
  }

  if(ActPos != curpos && digitalRead(Ref) == HIGH){
    reached = false;
    moveToPos();
  }
  
  if(digitalRead(Ready) == LOW){
    digitalWrite(Enable, LOW);
    delay(100);
    digitalWrite(Enable, HIGH);
  }
  
  if(refbool && digitalRead(Ready) == HIGH){
    digitalWrite(Enable, LOW);
    digitalWrite(Reset, HIGH);
    delay(100);
    digitalWrite(Enable, HIGH);
    delay(100);
    digitalWrite(Reset, LOW);
  }
  nh.spinOnce();
}

void moveToPos(){
  while(ActPos != curpos){
    if(ActPos - curpos > 0){
      if(ActPos - curpos >= 100){ //logarithmic moves in mm (100,10,1)
        digitalWrite(Adec, LOW);
        digitalWrite(Bdec, LOW);
        digitalWrite(Cdec, LOW);
        digitalWrite(DecPow, HIGH);
        curpos = curpos+100;
        //while(reached == false){
          delay(1000);
        //}
        if(ActPos - curpos != 0){
          reached = false;
        }        
      }else if(ActPos - curpos >= 10){
        digitalWrite(Adec, HIGH);
        digitalWrite(Bdec, LOW);
        digitalWrite(Cdec, LOW);
        digitalWrite(DecPow, HIGH);
        curpos = curpos+10;
        //while(reached == false){
          delay(400);
        //}
        if(ActPos - curpos != 0){
          reached = false;
        }        
      }/*else{
        digitalWrite(Adec, LOW);
        digitalWrite(Bdec, HIGH);
        digitalWrite(Cdec, LOW);
        digitalWrite(DecPow, HIGH);        
        curpos = curpos+1;
        while(reached == false){
          delay(10);
        }
        if(ActPos - curpos != 0){
          reached = false;
        }        
      }*/
    }else if(ActPos - curpos < 0){
      if(ActPos - curpos <= -100){ //logarithmic moves in mm (100,10,1)
        digitalWrite(Adec, HIGH);
        digitalWrite(Bdec, HIGH);
        digitalWrite(Cdec, LOW);
        digitalWrite(DecPow, HIGH);
        curpos = curpos-100;
        //while(reached == false){
          delay(1000);
        //}
        if(ActPos - curpos != 0){
          reached = false;
        }                
      }else if(ActPos - curpos <= -10){
        digitalWrite(Adec, LOW);
        digitalWrite(Bdec, LOW);
        digitalWrite(Cdec, HIGH);
        digitalWrite(DecPow, HIGH);
        curpos = curpos-10;
        //while(reached == false){
          delay(400);
        //}
        if(ActPos - curpos != 0){
          reached = false;
        }                
      }/*else{
        digitalWrite(Adec, HIGH);
        digitalWrite(Bdec, LOW);
        digitalWrite(Cdec, HIGH);
        digitalWrite(DecPow, HIGH);
        curpos = curpos-1;        
        while(reached == false){
          delay(10);
        }
        if(ActPos - curpos != 0){
          reached = false;
        }        
      }*/
    }else{
      return; //This should never happen, it means that we think we reached the goal, but we didn't
    }
    
  }

  
}


void goHome(){
  //while(reached == false){
    digitalWrite(Adec, HIGH);
    digitalWrite(Bdec, HIGH);
    digitalWrite(Cdec, HIGH);
    digitalWrite(DecPow, HIGH);
  //}
}

void setReached(){
  reached = true;
  digitalWrite(DecPow, LOW);
}
