#include <ros.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Int8.h>
/*
 * rosserial Publisher Example
 * Prints "hello world!"
 */


ros::NodeHandle nh;

const int suctionAnglePot = 0;

const int suctionAngleDir1 = 2;
const int suctionAngleDir2 = 3;
const int suctionAngleEn = 4;

float suctionAngleSense= -9999.0;

float desiredSense = 9999.0;

boolean g_moveAngle = false;
/* Calibration Parameters */


//std_msgs::Float32 suctionAngle;
std_msgs::Int8 suctionAngleCmd;

//ros::Publisher suctionAnglePub("suctionAngle", &suctionAngle);

int g_suctionAngleCmd = 90;

void suctionAngleCmdCB(const std_msgs::Int8& msg){
  g_suctionAngleCmd = msg.data;
  g_moveAngle = true;
}
ros::Subscriber<std_msgs::Int8> suctionAngleCmdSub("suctionAngleCmd", suctionAngleCmdCB);

void setup()
{
  nh.initNode();
 
  nh.subscribe(suctionAngleCmdSub);
  
  //nh.advertise(suctionAnglePub);
  
  pinMode(suctionAngleDir1, OUTPUT);
  pinMode(suctionAngleDir2, OUTPUT);
  pinMode(suctionAngleEn, OUTPUT);
  pinMode(A0, INPUT); //suctionHeadPot
  
}

void loop()
{
  if(g_moveAngle)
    suctionAngleMove(g_suctionAngleCmd);
  
  
  delay(50);   
  nh.spinOnce();

}




void suctionAngleMove(int cmd){
  suctionAngleSense = analogRead(suctionAnglePot);
  digitalWrite(suctionAngleEn, HIGH);
  
  desiredSense = 134.05+2.786*cmd+.02487*cmd*cmd;  //This is a hand tuned estimate, a better equation may be necessary/preferred

  /*
  if(cmd == 0){
    desiredSense = 105; //was 100
  }else if(cmd < 0){
    desiredSense = 100; //was 100
  }else if(cmd == 15){
    desiredSense = 135; //was 135
  }else if(cmd == 30){
    desiredSense = 165; //was 170
  }else if(cmd == 45){
    desiredSense = 205; //was 205
  }else if(cmd == 60){
    desiredSense = 240; //was 240, 
  }else if(cmd == 75){
    desiredSense = 270; //was 270, 
*/
  if (cmd <= 0){
    desiredSense = 125;
   }else if(cmd >= 90){
    desiredSense = 595; //was 300
   }
  
  
  if(suctionAngleSense < desiredSense){  //The basic idea is that we determine whether we are above or below, then move until we aren't
    while (suctionAngleSense < (desiredSense )){
      suctionAngleSense = analogRead(suctionAnglePot);
      
      digitalWrite(suctionAngleDir1, HIGH);
      digitalWrite(suctionAngleDir2, LOW);
    }
  }else if(suctionAngleSense > desiredSense){
    while (suctionAngleSense > (desiredSense)){
      suctionAngleSense = analogRead(suctionAnglePot);
      
      digitalWrite(suctionAngleDir1, LOW);
      digitalWrite(suctionAngleDir2, HIGH);
      
    }
  }else{}
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
   digitalWrite(suctionAngleDir1, HIGH);
  digitalWrite(suctionAngleDir2, HIGH);
  delay(10);
  digitalWrite(suctionAngleEn, LOW);
  g_moveAngle = false;
}
