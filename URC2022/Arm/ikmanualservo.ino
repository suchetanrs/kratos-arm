#include <ros.h>
#include <sensor_msgs/Joy.h>
#include <std_msgs/String.h>
#include <geometry_msgs/Point.h>
#include <geometry_msgs/Twist.h>
#include <Servo.h>
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

#define trigpin 24
#define ldrPin A2
#define POT4 A1
#define DIR4 6
#define PWM4 5 
#define DIR6 10                   //1 - 6inch
#define PWM6 11                   //2 - 4inch
#define servo1pin 9
#define servo2pin 8
#define servo3pin 7
#define servo4pin 4


geometry_msgs::Twist vels1;
geometry_msgs::Twist vels2;
//ros::Publisher pub1("velocities1", &vels1); //publisher1 to monitor outputs sent to the actuators
ros::Publisher pub2("velocities2", &vels2); //publisher2 to monitor outputs sent to the actuators
ros::NodeHandle n;


int i=0;
int j=0;
int k=0;
int m=0;
int count=0;
float potent=0;
int vel4=0;
int vel6=0;
float rackdist;
int drn1=0;
int drn2=0;
int flag1=0;
int flag2=0;
float initial_length4,final_length4,final_length6,initial_length6,initial_length4f; //final_length is the length coming from python file
                                                                                    //initial_length is the length coming from feedback
                                                                                    //initial_length4f is the length after scaling potentiometer value
int direction6inch=2;
int flagoff=0;
int flagon=0;
float masterflag;
float servo11,servo22,servo33;
int servofun=0;

void callback2(const geometry_msgs::Twist& msg){
  final_length4=msg.linear.x;
  final_length6=msg.linear.y;
  masterflag=msg.linear.z;
  servo11=msg.angular.x;
  servo22=msg.angular.y;
  servo33=msg.angular.z;
 }



ros::Subscriber<geometry_msgs::Twist>sub2("final_ext",callback2);


float rackfeedback(){
  n.spinOnce();
int ldrstatus=digitalRead(ldrPin);
if(ldrstatus==1){
  flagoff=1;
}
if(ldrstatus==0){
  flagon=1;
}

if(flagoff==1){
  int ldrstatus2=digitalRead(ldrPin);
  if(ldrstatus2==0){
    if(direction6inch==1){
      count++;
      flagoff=0;
    }
    if(direction6inch==0){
      count--;
      flagoff=0;
    }
  }
}
if(flagon==1){
  int ldrstatus2=digitalRead(ldrPin);
  if(ldrstatus2==1){
      if(direction6inch==1){
      count++;
      flagon=0;
    }
    if(direction6inch==0){
      count--;
      flagon=0;
    }
  }
}
  
  float distance=count*0.2;
  return distance;
  n.spinOnce();
}


void calculations(){
  n.spinOnce();
  potent=analogRead(POT4);
  rackdist=rackfeedback();
  initial_length6=rackdist+35.3;
  initial_length4f=((potent-50.0)/(535.0-50.0))*((10.2)-0);
  initial_length4=initial_length4f+35.04;
  n.spinOnce();
}



void ikconditions(){  
  n.spinOnce();
  /*for 4 inch linear actuator*/
  if(final_length4>(initial_length4 + 0.1))
  {
    digitalWrite(DIR4,HIGH);
    vel4=255;
    i=41;
  }
  else if(final_length4<(initial_length4-0.1))
  {
    digitalWrite(DIR4,LOW);
    vel4=255;
    i=42;
  }
  else
  {
    vel4=0;
    i=43;
  }

 /*for 6 inch linear actuator*/

 if(final_length6>(initial_length6 + 0.25))
  {j=61;
  direction6inch=1;
    digitalWrite(DIR6,direction6inch);
    //vel6=125; 
    vel6=255;
  }
  else if(final_length6<(initial_length6-0.25))
  {
    j=62;
    direction6inch=0;
    digitalWrite(DIR6,direction6inch);
    //vel6=125;
    vel6=255;
  }
  else
  {
    j=63;
    vel6=0;
  }

  analogWrite(PWM4,vel4);
  analogWrite(PWM6,vel6);
  n.spinOnce();
  
}

void manualcontrol(){
  vel4=1000;
  //float x = msg.linear.x;  //
  if(final_length4>0.8){
    digitalWrite(DIR4,LOW);//extension(blue-right) 
    vel4 = 254;
    i=2;
  }
  else if(final_length4<-0.8){
    digitalWrite(DIR4,HIGH);//retraction
    vel4 = 254;
    i=1;
  }
  else{
    vel4=0;
  }
  //float y = msg.linear.y;// extension(tape right)
  if(final_length6>0.8){
    digitalWrite(DIR6,LOW);
    vel6 = 254;
    i=2;
  }
  else if(final_length6<-0.8){
    digitalWrite(DIR6,HIGH);//retraction
    vel6 = 254;
    i=1;
  }
  else{
    vel6 = 0;
  }
  if(servo11==1){
    servo1.write(180);
    delay(200);
    servo1.write(90);
    servofun=1;
  }
  else if(servo11==-1){
    servo1.write(0);
    delay(200);
    servo1.write(90);
    servofun=2;
  }
  else if(servo22==1){
    servo2.write(180);
    delay(200);
    servo2.write(90);
    servofun=3;
  }
  else if(servo22==-1){
    servo2.write(0);
    delay(200);
    servo2.write(90);
    servofun=4;
  }
//  else if(servo33<-0.8){
//    servo3.write(180);
//    delay(100);
//    servo3.write(90);
//    servofun=5;
//  }
//  else if(servo33>0.8){
//    servo3.write(0);
//    delay(100);
//    servo3.write(90);
//    servofun=6;
//  }
  else{
  servo1.write(90);
  servo2.write(90);
  servo3.write(90);
  servofun=7;
  }
  analogWrite(PWM4,vel4);
  analogWrite(PWM6,vel6);
}


void setup() {
  pinMode(DIR4,OUTPUT);
  pinMode(PWM4,OUTPUT);
  pinMode(DIR6,OUTPUT);
  pinMode(PWM6,OUTPUT);
  pinMode(trigpin,OUTPUT);
  pinMode(ldrPin,INPUT);
  pinMode(8,OUTPUT);
  pinMode(POT4,INPUT);
  pinMode(9,OUTPUT);
  pinMode(2,OUTPUT);
  pinMode(26,OUTPUT);
  pinMode(51,OUTPUT);
  pinMode(53,OUTPUT);
  digitalWrite(8,HIGH);
  digitalWrite(26,LOW);
  digitalWrite(51,HIGH);
  digitalWrite(53,LOW);
  digitalWrite(9,HIGH);
  digitalWrite(2,HIGH);
  servo1.attach(servo1pin);
  servo2.attach(servo2pin);
  servo3.attach(servo3pin);
  servo4.attach(servo4pin);

//  digitalWrite(DIR4,LOW);
//  digitalWrite(PWM4,HIGH); //sending 6 inch actuator back
//  delay(10000);
//  digitalWrite(PWM4,LOW); //sending 6 inch actuator back
  n.initNode();
//  n.advertise(pub1);
  n.advertise(pub2);
  n.subscribe(sub2);
}


int counter=0;

void loop() {
  analogWrite(PWM4,vel4);
  analogWrite(PWM6,vel6);

  n.spinOnce();
  if(masterflag==1){
    calculations();
    manualcontrol();
  }
  if(masterflag==-1){
  calculations(); //to get feedback and map the potentiometer and rackfeedback feedback to their lengths
  ikconditions();
  }
  counter++;

  //values being published for monitoring
//  vels1.linear.x=vel4;
//  vels1.linear.y=vel6;
//  vels1.linear.z=i;
//  vels1.angular.x=j;
//  vels1.angular.y=initial_length4;
//  vels1.angular.z=initial_length6;
//
//  
//  vels2.linear.x=final_length4;
//  vels2.linear.y=final_length6;
//  vels2.linear.z=potent;
//  vels2.angular.x=rackdist;
//  vels2.angular.y=initial_length4f;
//  vels2.angular.z=counter;

  vels2.linear.x=final_length4;
  vels2.linear.y=final_length6;
  vels2.linear.z=rackdist;
  vels2.angular.x=servofun;
  vels2.angular.y=initial_length4;
  vels2.angular.z=initial_length6;
  
//  pub1.publish(&vels1);
  pub2.publish(&vels2);
  n.spinOnce();
}
