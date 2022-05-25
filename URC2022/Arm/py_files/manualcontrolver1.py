#! /usr/bin/env python3

import rospy
from std_msgs.msg import *
from sensor_msgs.msg import *
from geometry_msgs.msg import *

def cb(data):
    print("ok1")
    joy_value1=data.axes[0]
    joy_value2=data.axes[1]
    joy_value3=data.axes[3]
    joy_value4=data.axes[4]
    obj = Twist()
    obj.linear.x=joy_value1
    obj.linear.y=joy_value2
    obj.linear.z=1
    obj.angular.x=joy_value3
    obj.angular.y=joy_value4
    obj.angular.z=1
    pub.publish(obj)
    # rate.sleep()

rospy.init_node("joyinput",anonymous=True)
# rate = rospy.Rate(20)
pub=rospy.Publisher("manual_control",Twist,queue_size=10)
print("ok")
sub=rospy.Subscriber("joy", Joy, cb)
rospy.spin()
