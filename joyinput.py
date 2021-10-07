#! /usr/bin/env python3

import rospy
from std_msgs.msg import *
from sensor_msgs.msg import *
from geometry_msgs.msg import *

joy_value=0

def cb(data):
    global joy_value
    print("ok1")
    joy_value=data.axes[1]
    value_scaled=joy_value*100
    obj = Twist()
    obj.linear.x=value_scaled
    pub.publish(obj)

rospy.init_node("joyinput",anonymous=True)
pub=rospy.Publisher("cmd_vel",Twist,queue_size=10)
print("ok")
sub=rospy.Subscriber("joy", Joy, cb)
rospy.spin()
