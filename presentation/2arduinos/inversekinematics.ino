#include <ros.h>
#include <sensor_msgs/Joy.h>
#include <std_msgs/String.h>
#include <geometry_msgs/Point.h>
#include <geometry_msgs/Twist.h>

#define trigpin 24
#define echopin 22
#define POT4 A4
#define DIR4 10
#define PWM4 11 
#define DIR6 3                   //1 - 6inch
#define PWM6 4                   //2 - 4inch


geometry_msgs::Twist vels1;
geometry_msgs::Twist vels2;
ros::Publisher pub1("velocities1", &vels1); //publisher1 to monitor outputs sent to the actuators
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
float ultrsnd;
int drn1=0;
int drn2=0;

float initial_length4,final_length4,final_length6,initial_length6,initial_length4f; //final_length is the length coming from python file
                                                                                    //initial_length is the length coming from feedback
                                                                                    //initial_length4f is the length after scaling potentiometer value


void callback2(const geometry_msgs::Point& msg){
  final_length4=msg.x;
  final_length6=msg.y;
 }



ros::Subscriber<geometry_msgs::Point>sub2("final_ext",callback2);


float ultrasound(){
  digitalWrite(trigpin, LOW);
delayMicroseconds(2);
digitalWrite(trigpin, HIGH);// Sets the trigPin on HIGH state for 10 micro seconds
delayMicroseconds(10);
digitalWrite(trigpin, LOW);
float duration = pulseIn(echopin, HIGH);// Reads the echoPin, returns the sound wave travel time in microseconds
// Calculating the distance
float distance= ((duration*0.034/2));
return distance;
}



void calculations(){
  potent=analogRead(POT4);
  ultrsnd=ultrasound();
  initial_length6=ultrsnd+11.50+2.30;
  initial_length4f=((potent-50.0)/(535.0-50.0))*((10.2)-0);
  initial_length4=initial_length4f+23.50;
}



void ikconditions(){  
  /*for 4 inch linear actuator*/
  if(final_length4>(initial_length4 + 0.75))
  {
    digitalWrite(DIR4,HIGH);
    vel4=125;
    i=41;
  }
  else if(final_length4<(initial_length4-0.75))
  {
    digitalWrite(DIR4,LOW);
    vel4=125;
    i=42;
  }
  else
  {
    vel4=0;
    i=43;
  }

 /*for 6 inch linear actuator*/

 if(final_length6>(initial_length6 + 0.3))
  {j=61;
    digitalWrite(DIR6,HIGH);
    //vel6=125; 
    vel6=125;
  }
  else if(final_length6<(initial_length6-0.3))
  {
    j=62;
    digitalWrite(DIR6,LOW);
    //vel6=125;
    vel6=125;
  }
  else
  {
    j=63;
    vel6=0;
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
  pinMode(echopin,INPUT);
  pinMode(8,OUTPUT);
  pinMode(POT4,INPUT);
  pinMode(26,OUTPUT);
  pinMode(51,OUTPUT);
  pinMode(53,OUTPUT);
  digitalWrite(8,HIGH);
  digitalWrite(26,LOW);
  digitalWrite(51,HIGH);
  digitalWrite(53,LOW);

  
  n.initNode();
  n.advertise(pub1);
  n.advertise(pub2);
  n.subscribe(sub2);
}




void loop() {
  analogWrite(PWM4,vel4);
  analogWrite(PWM6,vel6);


  calculations(); //to get feedback and map the potentiometer and ultrasound feedback to their lengths
  ikconditions();


  //values being published for monitoring
  vels1.linear.x=vel4;
  vels1.linear.y=vel6;
  vels1.linear.z=i;
  vels1.angular.x=j;
  vels1.angular.y=initial_length4;
  vels1.angular.z=initial_length6;

  
  vels2.linear.x=final_length4;
  vels2.linear.y=final_length6;
  vels2.linear.z=potent;
  vels2.angular.x=ultrsnd;
  vels2.angular.y=initial_length4f;
  vels2.angular.z=0;
  
  pub1.publish(&vels1);
  pub2.publish(&vels2);
  n.spinOnce();
}
