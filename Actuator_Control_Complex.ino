//#include <ros.h>
//#include <std_msgs/Float32.h>
//#include <std_msgs/Int8.h>
//#include <math.h>


//Inputs (11) blue wires
const int MoCom = 20; //interrupt
const int StAck = 24;
const int Stopped = 26;
const int Moving = 28;
const int Error = 30;
const int Par1 = 32;
const int Par2 = 34;
const int Zone = 36;
const int Refed = 38;
const int Ready = 40;
const int Torque = 42;

//Outputs (11) yellow wires
const int Rec0 = 31;
const int Rec1 = 33;
const int Rec2 = 35;
const int Rec3 = 37;
const int Rec4 = 39;
const int Start = 41;
const int Pause = 43;
const int Mode = 45; //you will never use this
const int Brake = 47;
const int Enable = 49;
const int Reset = 51;

int ActRef, ActPos;
float curpos;  
boolean reached, refbool, lastadjust;
/*
void ActRefCB( const std_msgs::Int8& msg){
  ActRef = msg.data;
}
ros::Subscriber<std_msgs::Int8> ActRefSub("ActRef", ActRefCB);

void ActPosCB( const std_msgs::Float32& msg){
  ActPos = (int)round(1000*msg.data);
}
ros::Subscriber<std_msgs::Float32> ActPosSub("ActPos", ActPosCB);
*/

void setup() {
  // put your setup code here, to run once:
  int inpins[] = {StAck,Stopped,Moving,Error,Par1,Par2,Zone,Refed,Ready,Torque};
  for (int i = 0; i < (sizeof(inpins)/sizeof(int)); i++){
    pinMode(inpins[i], INPUT);
  }
  attachInterrupt(3, MoveComplete, RISING); //interrupt hardocded

  int outpins[] = {Rec0, Rec1, Rec2, Rec3, Rec4, Start, Pause, Mode, Brake, Enable, Reset};
  for (int i = 0; i < (sizeof(outpins)/sizeof(int)); i++){
    pinMode(outpins[i], OUTPUT);
    digitalWrite(outpins[i], HIGH); //We are active LOW
  }
  

  ActRef = 0;
  ActPos = -1000;
  curpos = -1000;
  reached = true;
  digitalWrite(Enable, HIGH);
  refbool = false;
  lastadjust = false;
}

void loop() {
  // put your main code here, to run repeatedly:
  if(ActRef == 1 && refbool == false){
    executeRecord(0);
    refbool = true;
  }
  
  if(abs(ActPos-curpos) >= 10){ //min increment 10mm
    reached = false;
    executeMove();
  }
 //spin 
}

void executeMove(){
  if(abs(ActPos-curpos) >= 40){ //40 mm increments for absolute position
    int rec = int((ActPos+500)/40) + 1;
    executeRecord(rec); //0 for actuator is -500 for Ros. actutator works in increments of 40mm. starts at Rec 1 (not 0)
    curpos = int(ActPos/40)*40;
  }else if(ActPos-curpos > 0){
    if(lastadjust){
      executeRecord(26);
    }else{
      executeRecord(28);
    }
    lastadjust = !lastadjust;
    curpos = curpos + 10;
  }else if(ActPos-curpos < 0){
    if(lastadjust){
      executeRecord(27);
    }else{
      executeRecord(29);
    }
    lastadjust = !lastadjust;
    curpos + curpos - 10;
  }
}

void executeRecord(int rec){ //We are active LOW
  if(rec >= 16){
    rec = rec - 16;
    digitalWrite(Rec4, LOW);
  }
  if(rec >= 8){
    rec = rec - 8;
    digitalWrite(Rec3, LOW);
  }
  if(rec >= 4){
    rec = rec - 4;
    digitalWrite(Rec2, LOW);
  }
  if(rec >= 2){
    rec = rec - 2;
    digitalWrite(Rec1, LOW);
  }
  if(rec >= 1){
    rec = rec - 1;
    digitalWrite(Rec0, LOW);
  }
  digitalWrite(Start, LOW);
  while(reached == false){
    //spin
  }
}

void MoveComplete(){
  reached = true;
  int outpins[] = {Rec0, Rec1, Rec2, Rec3, Rec4, Start};
  for (int i = 0; i < (sizeof(outpins)/sizeof(int)); i++){
    digitalWrite(outpins[i], HIGH); //We are active LOW
  }
}

