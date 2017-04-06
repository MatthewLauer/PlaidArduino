#include <ros.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Int8.h>

const int leftPressureSensorPin = 0;
const int rightPressureSensorPin = 1;

const int leftRelayPin = 3;
const int rightRelayPin = 6;

/* Calibration Parameters */
float contactSuctionPressure  = 50.0; //kpa
float nominalSuctionOFFPressure = 101.0; //kpa
float nominalSuctionOFFTolerance = 25.0; //kpa

ros::NodeHandle  nh;

std_msgs::Float32 suctionLeftPressureKpa, suctionRightPressureKpa;
std_msgs::Int8 suctionSystemStatus, suctionLeftFeedback, suctionRightFeedback;

ros::Publisher suctionLeftPressureKpaPub("suctionLeftPressureKpa", &suctionLeftPressureKpa);
ros::Publisher suctionRightPressureKpaPub("suctionRightPressureKpa", &suctionRightPressureKpa);
ros::Publisher suctionSystemStatusPub("suctionSystemStatus", &suctionSystemStatus);
ros::Publisher suctionLeftFeedbackPub("suctionLeftFeedback", &suctionLeftFeedback);
ros::Publisher suctionRightFeedbackPub("suctionRightFeedback", &suctionRightFeedback);

int g_leftRelayCmd, g_rightRelayCmd;

void leftRelayCmdCB( const std_msgs::Int8& msg){
  g_leftRelayCmd = msg.data;
}
ros::Subscriber<std_msgs::Int8> leftRelayCmdSub("leftRelayCmd", leftRelayCmdCB);

void rightRelayCmdCB( const std_msgs::Int8& msg){
  g_rightRelayCmd = msg.data;
}
ros::Subscriber<std_msgs::Int8> rightRelayCmdSub("rightRelayCmd", rightRelayCmdCB);

void actuateSuctionSystem();
int updateStatus(float);

void setup()
{
  nh.initNode();
  
  nh.advertise(suctionLeftPressureKpaPub);
  nh.advertise(suctionRightPressureKpaPub);
  
  nh.advertise(suctionRightFeedbackPub);
  nh.advertise(suctionLeftFeedbackPub);  
   
  nh.subscribe(leftRelayCmdSub);
  nh.subscribe(rightRelayCmdSub);
  
  nh.advertise(suctionSystemStatusPub);
  
  pinMode(leftRelayPin, OUTPUT);
  pinMode(rightRelayPin, OUTPUT);
  
  suctionSystemStatus.data = 0;

  pinMode(7, OUTPUT);
  digitalWrite(7, LOW);
  
}

void loop()
{
  // update pressure reading for display
  float leftkpa = analogRead(leftPressureSensorPin);
  leftkpa = ( (leftkpa/1023.0 - .03)  * 777.726);
  
  float rightkpa = analogRead(rightPressureSensorPin);
  rightkpa = ( (rightkpa/1023.0 - .03)  * 777.726);
  
  // checks global relay commands
  actuateSuctionSystem();
  
  /* Int8 1 - suction ON,   nominal pressure 
          2 - suction ON,   low pressure
          3 - suction OFF,  nominal pressure 
          4 - suction OFF,  bad pressure
  */
  suctionLeftFeedback.data  = updateLeftStatus(leftkpa);
  suctionLeftFeedbackPub.publish(&suctionLeftFeedback);
  suctionLeftPressureKpa.data  = leftkpa;
  suctionLeftPressureKpaPub. publish(&suctionLeftPressureKpa);
  
  suctionRightFeedback.data = updateRightStatus(leftkpa);
  suctionRightFeedbackPub.publish(&suctionRightFeedback);
  suctionRightPressureKpa.data  = rightkpa;
  suctionRightPressureKpaPub. publish(&suctionRightPressureKpa);
  
  suctionSystemStatus.data=1;
  suctionSystemStatusPub.publish(&suctionSystemStatus);
  
  nh.spinOnce();
  delay(25);
}

int updateLeftStatus(float kpa)
{
  int statusFlag = 0;
  
  if(g_leftRelayCmd > 0)
  { // Suction system is turned ON   
    if(kpa < contactSuctionPressure)
    { // Item has been grabbed by suction
      statusFlag = 2;
    }else
    { // Item has not been grabbed by suction
      statusFlag = 1;
    }   
  }else
  { // Suction system is turned OFF
     if(abs(kpa - nominalSuctionOFFPressure) < nominalSuctionOFFTolerance)
     { // Pressure is reading correctly
       statusFlag = 3;
     }else
     { // Pressure is not reading correctly
       statusFlag = 4;
     }   
  }
  return statusFlag;
}

int updateRightStatus(float kpa)
{
  int statusFlag = 0;
  
  if(g_rightRelayCmd > 0)
  { // Suction system is turned ON   
    if(kpa < contactSuctionPressure)
    { // Item has been grabbed by suction
      statusFlag = 2;
    }else
    { // Item has not been grabbed by suction
      statusFlag = 1;
    }   
  }else
  { // Suction system is turned OFF
     if(abs(kpa - nominalSuctionOFFPressure) < nominalSuctionOFFTolerance)
     { // Pressure is reading correctly
       statusFlag = 3;
     }else
     { // Pressure is not reading correctly
       statusFlag = 4;
     }   
  }
  return statusFlag;
}


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
