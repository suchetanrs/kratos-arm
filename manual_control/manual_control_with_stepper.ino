#include <ros.h>
#include <sensor_msgs/Joy.h>
#include <std_msgs/String.h>
#include <geometry_msgs/Point.h>
#include <geometry_msgs/Twist.h>
#define DIR4 10               //connections to be checked
#define PWM4 11
#define DIR6 3                   //1 6inch 2 4inch
#define PWM6 4
#define baserotationpwm 6
#define basedirection 7
#define bevel_right_pwm 8
#define bevel_left_pwm  9
#define bevel_right_drn 23
#define bevel_left_drn 2
#define stepper_step 5
#define stepper_drn 2
int steps=475;
#define POT1 A4
int i=0;
int j=0;
int k=0;
int m=0;
int u=0;
int potent=0;
int maxl=0;
int minl=0;
ros::NodeHandle n;
geometry_msgs::Twist vels;
ros::Publisher pub1("velocities", &vels);
int vel1,vel2,vel3,vel4,vel5,vel6;


void stepperopen(){
  digitalWrite(stepper_drn,HIGH);

  for(int p = 0; p < steps; p++)
  {
    u=1;
    digitalWrite(stepper_step, HIGH);
    delayMicroseconds(2000);
    digitalWrite(stepper_step, LOW);
    delayMicroseconds(2000);
  }
  
}
void stepperclose(){
  digitalWrite(stepper_drn,LOW);

  for(int p = 0; p < steps; p++)
  {
    u=2;
    digitalWrite(stepper_step, HIGH);
    delayMicroseconds(2000);
    digitalWrite(stepper_step, LOW);
    delayMicroseconds(2000);
  }
}


void callback(const geometry_msgs::Twist& msg)
{
  int potent=analogRead(POT1);
  float x = msg.linear.x;  //
  if(x>0.8){
    digitalWrite(DIR4,LOW);//extension(blue-right) 
    vel1 = 255;
    vel2=0;
    i=2;
  }
  else if(x<-0.8){
    digitalWrite(DIR4,HIGH);//retraction
    vel1 = 255;
    vel2=0;
    i=1;
  }
  else{
    vel1=0;
  }
  float y = msg.linear.y;// extension(tape right)
  if(y>0.8){
    digitalWrite(DIR6,LOW);
    vel2 = 255;
    vel1=0;
    i=2;
  }
  else if(y<-0.8){
    digitalWrite(DIR6,HIGH);//retraction
    vel2 = 255;
    vel1=0;
    i=1;
  }
  else{
    vel2 = 0;
  }
  float bsrot1=msg.angular.x;
  //float bsrot2=msg.angular.y;
  if(bsrot1==1){
    digitalWrite(basedirection,LOW);//extension(blue-right) 
    vel3 = 255;
    i=2;
    k++;
  }
  else if(bsrot1==-1){
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
 float gripper=msg.angular.y;
  if(gripper>0.8){
    stepperopen();
  }
  else if(gripper<-0.8){
    stepperclose();
  }
  else{
    u=0;
  }
  analogWrite(PWM4,vel1);
  analogWrite(PWM6,vel2);
  analogWrite(baserotationpwm,vel3);
  analogWrite(bevel_right_pwm,vel4);
  analogWrite(bevel_left_pwm,vel5);
  vels.linear.x=vel1;//4inch
  vels.linear.y=vel2;//6inch
  vels.linear.z=vel3;//baserot
  vels.angular.x=vel4;//bevelright
  vels.angular.y=vel5;//bevelleft
  vels.angular.z=u;
}
ros::Subscriber<geometry_msgs::Twist> sub1("cmd_vel", callback); 
void setup(){
  pinMode(DIR4,OUTPUT);
  pinMode(PWM4,OUTPUT);
  pinMode(DIR6,OUTPUT);
  pinMode(PWM6,OUTPUT);
  pinMode(baserotationpwm,OUTPUT);
  pinMode(basedirection,OUTPUT);
  pinMode(bevel_right_pwm,OUTPUT);
  pinMode(bevel_left_pwm,OUTPUT);
  pinMode(bevel_right_drn,OUTPUT);
  pinMode(bevel_left_drn,OUTPUT);
  pinMode(POT1,INPUT);
  pinMode(51,OUTPUT);
  pinMode(53,OUTPUT);
  pinMode(stepper_step,OUTPUT);
  pinMode(stepper_drn,OUTPUT);
  n.initNode();
  n.subscribe(sub1);
  n.advertise(pub1);
}

void loop(){
  digitalWrite(51,HIGH); //potentiometer feedbacks
  digitalWrite(53,LOW);
  pub1.publish(&vels);
  n.spinOnce();
}
