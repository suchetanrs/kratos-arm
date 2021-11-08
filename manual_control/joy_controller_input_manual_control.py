#! /usr/bin/env python3

import rospy
from std_msgs.msg import *
from sensor_msgs.msg import *
from geometry_msgs.msg import *

#joy_value1=0
#joy_value2=0

def cb(data):
    global joy_value
    print("ok1")
    joy_value1=data.axes[1]
    joy_value2=data.axes[0]
    value_scaled1=joy_value1
    value_scaled2=joy_value2
    base_motor1=data.axes[3]
    bevel_motor1=data.axes[6]
    bevel_motor2=data.axes[7]
    stepper_motor=data.axes[4]

    obj = Twist()
    obj.linear.x=value_scaled1
    obj.linear.y=value_scaled2
    obj.angular.x=base_motor1
    obj.angular.y=stepper_motor
    obj.linear.z=bevel_motor1
    obj.angular.z=bevel_motor2
    pub.publish(obj)

rospy.init_node("joyinput",anonymous=True)
pub=rospy.Publisher("cmd_vel",Twist,queue_size=10)
print("ok")
sub=rospy.Subscriber("joy", Joy, cb)
rospy.spin()
