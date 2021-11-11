#! /usr/bin/env python3

import rospy
from std_msgs.msg import *
from sensor_msgs.msg import *
from geometry_msgs.msg import *

def cb(data):
    global joy_value
    print("ok1")
    joy_value1=data.axes[3]
    joy_value2=data.axes[4]
    value_scaled1=joy_value1
    value_scaled2=joy_value2

    obj = Twist()
    obj.linear.x=value_scaled1
    obj.linear.y=value_scaled2
    pub.publish(obj)

rospy.init_node("joyinput",anonymous=True)
pub=rospy.Publisher("actuators",Twist,queue_size=10)
print("ok")
sub=rospy.Subscriber("joy", Joy, cb)
rospy.spin()
