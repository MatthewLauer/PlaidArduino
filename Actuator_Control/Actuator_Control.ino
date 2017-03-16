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
boolean refbool;
boolean reached;

std_msgs::Float32 ActPosFB;

ros::Publisher ActPosFBpub("ActPosFB", &ActPosFB);

void ActRefCB( const std_msgs::Int8& msg){
  ActRef = msg.data;
}
ros::Subscriber<std_msgs::Int8> ActRefSub("ActRef", ActRefCB);

void ActPosCB( const std_msgs::Float32& msg){
  ActPos = (int)round(1000*msg.data);
  if(ActPos > 490){
    ActPos = 490;
  }
  if(ActPos < -490){
    ActPos = -490;
  }
}
ros::Subscriber<std_msgs::Float32> ActPosSub("ActPos", ActPosCB);

void setup() {
  //nh.getHardware()->setBaud(115200);
  nh.initNode();

  nh.subscribe(ActPosSub);
  nh.subscribe(ActRefSub);
  nh.advertise(ActPosFBpub);
  pinMode(13, OUTPUT);


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
  ActPos = -500;
  curpos = -1000;
  reached = true;
  digitalWrite(Enable, HIGH);
  refbool = false;
}

void loop() {
  if(digitalRead(Ref) == LOW && ActRef == 1){// && digitalRead(Ready) == HIGH){
    digitalWrite(DecPow, LOW);
    reached = false;
    goHome();
    curpos = -500;
    delay(500);
  }

  if(ActPos != curpos && digitalRead(Ref) == HIGH){
    reached = false;
    moveToPos();
  }
  
  /*if(digitalRead(Ready) == LOW){
    digitalWrite(Enable, LOW);
    delay(100);
    digitalWrite(Enable, HIGH);
  }*/
  
  /*if(refbool && digitalRead(Ready) == HIGH){
    digitalWrite(Enable, LOW);
    digitalWrite(Reset, HIGH);
    delay(100);
    digitalWrite(Enable, HIGH);
    delay(100);
    digitalWrite(Reset, LOW);
  }*/
  nh.spinOnce();  
  ActPosFB.data = float(curpos)/1000.0;
  ActPosFBpub.publish(&ActPosFB);
  delay(50);
}

void moveToPos(){
  while(ActPos != curpos){
    if(ActPos - curpos > 0){
      if(ActPos - curpos >= 100){ //logarithmic moves in mm (100,10,1)
        digitalWrite(Adec, LOW);
        digitalWrite(Bdec, LOW);
        digitalWrite(Cdec, LOW);
        digitalWrite(DecPow, HIGH);
        while(reached == false){
          spinWait(100);
        }
        digitalWrite(13, LOW);
        reached = false;
        curpos = curpos+100;
      }else if(ActPos - curpos >= 10){
        digitalWrite(Adec, HIGH);
        digitalWrite(Bdec, LOW);
        digitalWrite(Cdec, LOW);
        digitalWrite(DecPow, HIGH);
        while(reached == false){
          spinWait(100);
        }
        digitalWrite(13, LOW);
        reached = false;
        curpos = curpos+10;
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
        while(reached == false){
          spinWait(100);
        }
        digitalWrite(13, LOW);
        reached = false;
        curpos = curpos-100;
      }else if(ActPos - curpos <= -10){
        digitalWrite(Adec, LOW);
        digitalWrite(Bdec, LOW);
        digitalWrite(Cdec, HIGH);
        digitalWrite(DecPow, HIGH);
        while(reached == false){
          spinWait(100);
        }
        digitalWrite(13, LOW);
        reached = false;
        curpos = curpos-10;

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
      return;
    }
  }

  
}

void spinWait(int time){
  for(int i = 0; i < time; i++){
    delay(10);
    nh.spinOnce();
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
  digitalWrite(13, HIGH);
  ActPosFB.data = float(curpos)/1000.0;
  ActPosFBpub.publish(&ActPosFB);
}
