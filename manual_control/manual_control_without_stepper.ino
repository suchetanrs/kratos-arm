#include <ros.h>
#include <sensor_msgs/Joy.h>
#include <std_msgs/String.h>
#include <geometry_msgs/Point.h>
#include <geometry_msgs/Twist.h>
#define DIR1 10               //connections to be checked
#define PWM1 11
#define DIR2 3                   //1 6inch 2 4inch
#define PWM2 4
#define baserotationpwm 6
#define basedirection 7
#define bevel_right_pwm 7
#define bevel_left_pwm  9
#define bevel_right_drn 23
#define bevel_left_drn  22
#define POT1 A4
int i=0;
int j=0;
int k=0;
int m=0;
int potent=0;
int maxl=0;
int minl=0;
ros::NodeHandle n;
geometry_msgs::Twist vels;
ros::Publisher pub1("velocities", &vels);
int vel1,vel2,vel3,vel4,vel5;
void callback(const geometry_msgs::Twist& msg)
{
  int potent=analogRead(POT1);
  float x = msg.linear.x;  //
  if(x>0.8){
    digitalWrite(DIR1,LOW);//extension(blue-right) 
    vel1 = 255;
    vel2=0;
    i=2;
  }
  else if(x<-0.8){
    digitalWrite(DIR1,HIGH);//retraction
    vel1 = 255;
    vel2=0;
    i=1;
  }
  else{
    vel1=0;
  }
  float y = msg.linear.y;// extension(tape right)
  if(y>0.8){
    digitalWrite(DIR2,LOW);
    vel2 = 255;
    vel1=0;
    i=2;
  }
  else if(y<-0.8){
    digitalWrite(DIR2,HIGH);//retraction
    vel2 = 255;
    vel1=0;
    i=1;
  }
  else{
    vel2 = 0;
  }
  float bsrot1=msg.angular.x;
  float bsrot2=msg.angular.y;
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
    digitalWrite(bevel_right_drn,HIGH);//extension(blue-right)
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
  analogWrite(PWM1,vel1);
  analogWrite(PWM2,vel2);
  analogWrite(baserotationpwm,vel3);
  analogWrite(bevel_right_pwm,vel4);
  analogWrite(bevel_left_pwm,vel5);
  vels.linear.x=vel4;
  vels.linear.y=vel5;
  vels.linear.z=m;
}
ros::Subscriber<geometry_msgs::Twist> sub1("cmd_vel", callback); 
void setup(){
  pinMode(DIR1,OUTPUT);
  pinMode(PWM1,OUTPUT);
  pinMode(DIR2,OUTPUT);
  pinMode(PWM2,OUTPUT);
  pinMode(baserotationpwm,OUTPUT);
  pinMode(basedirection,OUTPUT);
  pinMode(bevel_right_pwm,OUTPUT);
  pinMode(bevel_left_pwm,OUTPUT);
  pinMode(bevel_right_drn,OUTPUT);
  pinMode(bevel_left_drn,OUTPUT);
  pinMode(POT1,INPUT);

  n.initNode();
  n.subscribe(sub1);
  n.advertise(pub1);
}

void loop(){
  
  pub1.publish(&vels);
  n.spinOnce();
}
