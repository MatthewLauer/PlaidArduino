#include <ros.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Int8.h>
/*
 * rosserial Publisher Example
 * Prints "hello world!"
 */


ros::NodeHandle nh;

const int suctionAnglePot = 0;
const int leftPressureSensorPin = 2;
const int rightPressureSensorPin = 1;

const int leftRelayPin = 3;
const int rightRelayPin = 6;
const int suctionAngleDir1 = 2;
const int suctionAngleDir2 = 3;
const int suctionAngleEn = 4;

/* Calibration Parameters */
float contactSuctionPressure  = 50.0; //kpa
float nominalSuctionOFFPressure = 101.0; //kpa
float nominalSuctionOFFTolerance = 25.0; //kpa


std_msgs::Float32 suctionRightPressureKpa, suctionAngle;
std_msgs::Int8 suctionSystemStatus, suctionLeftFeedback, suctionRightFeedback, suctionAngleCmd;

ros::Publisher suctionRightPressureKpaPub("suctionRightPressureKpa", &suctionRightPressureKpa);
ros::Publisher suctionAnglePub("suctionAngle", &suctionAngle);

int g_leftRelayCmd, g_rightRelayCmd, g_suctionAngleCmd = 90;

void rightRelayCmdCB( const std_msgs::Int8& msg){
  g_rightRelayCmd = msg.data;
}
ros::Subscriber<std_msgs::Int8> rightRelayCmdSub("rightRelayCmd", rightRelayCmdCB);

void suctionAngleCmdCB( const std_msgs::Int8& msg){
  g_suctionAngleCmd = msg.data;
}
ros::Subscriber<std_msgs::Int8> suctionAngleCmdSub("suctionAngleCmd", suctionAngleCmdCB);


void actuateSuctionSystem();

void setup()
{
  nh.initNode();
 
  nh.advertise(suctionRightPressureKpaPub);
  
  nh.subscribe(rightRelayCmdSub);
  nh.subscribe(suctionAngleCmdSub);
  
  nh.advertise(suctionAnglePub);
  
  pinMode(leftRelayPin, OUTPUT);
  pinMode(rightRelayPin, OUTPUT);
  
  pinMode(suctionAngleDir1, OUTPUT);
  pinMode(suctionAngleDir2, OUTPUT);
  pinMode(suctionAngleEn, OUTPUT);
  
    pinMode(7, OUTPUT);
  digitalWrite(7, LOW);
  
}

void loop()
{
  // update pressure reading for display
  float rightkpa = analogRead(rightPressureSensorPin);
  rightkpa = ( (rightkpa/1023.0 - .03)  * 777.726);
  
//  // checks global relay commands
  suctionAngleMove(g_suctionAngleCmd);
  
  /* Int8 1 - suction ON,   nominal pressure 
          2 - suction ON,   low pressure
          3 - suction OFF,  nominal pressure 
          4 - suction OFF,  bad pressure
  */
  suctionRightPressureKpa.data  = rightkpa;
  suctionRightPressureKpaPub. publish(&suctionRightPressureKpa);
  
  float suctionAngleSense = analogRead(suctionAnglePot);
  suctionAngle.data  = suctionAngleSense;
  suctionAnglePub.publish(&suctionAngle);
  
  actuateSuctionSystem();
   
  nh.spinOnce();
  delay(25);
}

//void loop()
//{
//  // update pressure reading for display
//  float rightkpa = analogRead(rightPressureSensorPin);
//  rightkpa = ( (rightkpa/1023.0 - .03)  * 777.726);
//  
//  // checks global relay commands
//  suctionAngleMove(g_suctionAngleCmd);
//  
//  /* Int8 1 - suction ON,   nominal pressure 
//          2 - suction ON,   low pressure
//          3 - suction OFF,  nominal pressure 
//          4 - suction OFF,  bad pressure
//  */
//  suctionRightPressureKpa.data  = rightkpa;
//  suctionRightPressureKpaPub. publish(&suctionRightPressureKpa);
//  
//  float suctionAngleSense = analogRead(suctionAnglePot);
//  suctionAngle.data  = suctionAngleSense;
//  suctionAnglePub.publish(&suctionAngle);
//
//  str_msg.data = hello;
//  chatter.publish( &str_msg );
//  
//  nh.spinOnce();
//  delay(250);
//  
//}
//
//
void actuateSuctionSystem()
{
  if(g_leftRelayCmd > 0)
  { // Turn suction system ON
    digitalWrite(leftRelayPin, HIGH);
  }else
  { // Turn suction system OFF
    digitalWrite(leftRelayPin, LOW);
  }
  
  if(g_rightRelayCmd > 0)
  { // Turn suction system ON
    digitalWrite(rightRelayPin, HIGH);
  }else
  { // Turn suction system OFF
    digitalWrite(rightRelayPin, LOW);
  }    
  
}


void suctionAngleMove(int cmd){
  float suctionAngleSense = analogRead(suctionAnglePot);
  digitalWrite(suctionAngleEn, HIGH);
  /*if(cmd == 90){
    while (suctionAngleSense < 300){
      suctionAngleSense = analogRead(suctionAnglePot);
      
      digitalWrite(suctionAngleDir1, HIGH);
      digitalWrite(suctionAngleDir2, LOW);
      
    }
  }else if(cmd == 0){
    while (suctionAngleSense > 80){
      suctionAngleSense = analogRead(suctionAnglePot);
      
      digitalWrite(suctionAngleDir1, LOW);
      digitalWrite(suctionAngleDir2, HIGH);
      
    }
    
  }else{}
  */
  digitalWrite(suctionAngleEn, LOW);
}
