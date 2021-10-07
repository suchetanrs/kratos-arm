#include <ros.h>
#include <ArduinoHardware.h>
#include<geometry_msgs/Twist.h>

ros::NodeHandle nh;

int speed_pin=7;
int direction=10;
void callback( const geometry_msgs::Twist& data)
{
    int motor_speed=data.linear.x;
    if(motor_speed>0)
    {
        digitalWrite(direction,0);
        analogWrite(speed_pin,motor_speed);
    }
    if(motor_speed<0)
    {
        digitalWrite(direction,1);
        analogWrite(speed_pin,motor_speed);
    }
    if(motor_speed==0)
    {
        analogWrite(speed_pin,0);
    }
}

ros::Subscriber<geometry_msgs::Twist> sub("cmd_vel", &messageCb);

void setup()
{
    pinMode(speed_pin, OUTPUT);
    pinMode(direction, OUTPUT);
    nh.initNode();
    nh.subscribe(sub);
}

void loop()
{
    nh.spinOnce();
}
