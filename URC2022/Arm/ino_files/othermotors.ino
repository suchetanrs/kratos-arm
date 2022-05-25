#include <ros.h>
#include <sensor_msgs/Joy.h>
#include <std_msgs/String.h>
#include <geometry_msgs/Point.h>
#include <geometry_msgs/Twist.h>

#define baserotationpwm 6
#define basedirection 7
#define bevel_right_pwm 10
#define bevel_left_pwm  9
#define bevel_right_drn 5
#define bevel_left_drn  12
const int dirPin = 4; // can be 4 and 3 as well
const int stepPin = 11; // can be 11 and 2 as well
int stepp=0;
const int stepsPerRevolution = 100;

int i=0;
int j=0;
int k=0;
int m=0;
int potent=0;
int maxl=0;
int minl=0;
ros::NodeHandle n;
geometry_msgs::Twist vels;

ros::Publisher pub1("velocitieso", &vels);

int vel1,vel2,vel3,vel4,vel5;

void callback(const geometry_msgs::Twist& msg)
{
  float bsrot1=msg.linear.x;
  float bsrot2=msg.linear.y;
  if(bsrot2==1){
    digitalWrite(basedirection,LOW);//extension(blue-right) 
    vel3 = 255;
    i=2;
    k++;
  }
  else if(bsrot1==1){
    digitalWrite(basedirection,HIGH);//retraction
    vel3 = 255;
    i=1;
    k++;
  }
  else{
    vel3=0;
  }
  float bevelrot1=msg.linear.z;
  float bevelrot2=msg.angular.z;
  if(bevelrot1>0.9){
    digitalWrite(bevel_right_drn,LOW);//extension(blue-right)
    digitalWrite(bevel_left_drn,HIGH);
    vel4 = 255;
    vel5 = 255;
    
    m=10;
  }
  else if(bevelrot1<-0.9){
    digitalWrite(bevel_right_drn,HIGH)  ;//extension(blue-right)
    digitalWrite(bevel_left_drn,LOW);
    vel4 = 255;
    vel5 = 255;
   
    m=20;
  }
  else if(bevelrot2>0.9){
    digitalWrite(bevel_right_drn,HIGH);//extension(blue-right)
    digitalWrite(bevel_left_drn,HIGH);
    vel4 = 255;
    vel5 = 255;
 
    m=30;
  }
  else if(bevelrot2<-0.9){
    digitalWrite(bevel_right_drn,LOW);//extension(blue-right)
    digitalWrite(bevel_left_drn,LOW);
    vel4 = 255;
    vel5 = 255;
  

    m=40;
  }
  else{
    vel4 = 0;
    vel5 = 0;
    digitalWrite(bevel_right_drn,LOW);
    digitalWrite(bevel_left_drn,LOW);
    m=100;
  }
  float steppermotor1=0;
  float steppermotor2=0;
  steppermotor1=msg.angular.x;
  steppermotor2=msg.angular.y;
  if(steppermotor1==1){
      digitalWrite(dirPin, HIGH);
      stepp=101;

  // Spin motor slowly
  for(int x = 0; x < stepsPerRevolution; x++)
  {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(2000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(2000);
  }
  }
  else if(steppermotor2==1){
  digitalWrite(dirPin, LOW);
  stepp=200;

  // Spin motor slowly
  for(int x = 0; x < stepsPerRevolution; x++)
  {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(2000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(2000);
  }
  }
  analogWrite(baserotationpwm,(vel3/2)); 
  analogWrite(bevel_right_pwm,(vel4/2));
  analogWrite(bevel_left_pwm,(vel5/2));
  vels.linear.x=0;
  vels.linear.y=i;
  vels.linear.z=stepp;

  vels.angular.x=vel4;
  vels.angular.y=vel5;
  vels.angular.z=vel3;
}
ros::Subscriber<geometry_msgs::Twist> sub1("othermotors2", callback); 
void setup(){
  pinMode(baserotationpwm,OUTPUT);
  pinMode(basedirection,OUTPUT);
  pinMode(bevel_right_pwm,OUTPUT);
  pinMode(bevel_left_pwm,OUTPUT);
  pinMode(bevel_right_drn,OUTPUT);
  pinMode(bevel_left_drn,OUTPUT);
  pinMode(dirPin,OUTPUT);
  pinMode(stepPin,OUTPUT);

  n.initNode();
  n.subscribe(sub1);
  n.advertise(pub1);
}

void loop(){
  
  pub1.publish(&vels);
  n.spinOnce();
}
