#!/usr/bin/env python3

import rospy
import math
from sensor_msgs.msg import Joy
from geometry_msgs.msg import Point
# import time

class Kin():
    #lengths in meters
    # pulished in cm 

    def __init__(self):
        self.x = 54.50 * (10**-2)
        self.y = 1.0 * (10**-2)
        self.l4 = 30.00 * (10**-2)
        self.l6 = 26.00 * (10**-2)
        self.k4 = 9 * (10**-2)
        self.k6 = 22.0* (10**-2)
        self.h4 = 19.5 * (10**-2)
        self.h6 = 7 * (10**-2)
        self.a4 = 22.0 * (10**-2)           #to be initialised with given value
        self.a6 = 26.5 * (10**-2)           #to be initialised with given value

        self.q6 = 0        #to be measured again
        self.q4 = 0          #to be measured again
        self.f = 60
        self.LL4=0
        self.LL6=0
        #self.x_max=0                        #to be implemented
        #self.y_max=0                        #to be implemented
        self.pub=rospy.Publisher("final_ext",Point,queue_size=5)
        self.sub=rospy.Subscriber("joy",Joy,self.callback)


    
    def callback(self,msg):
        #map buttons and axes to the function calls
        if(msg.axes[0] < -0.5):
            self.x += 0.001
        elif(msg.axes[0] > 0.5):
            self.x -= 0.001
        if(msg.axes[1] > 0.5):
            self.y += 0.001
        elif(msg.axes[1] < -0.5):
            self.y -= 0.001
        self.inverse_kinematics()
        #print(self.a4, self.a6)
        self.pub.publish((self.a4*100), (self.a6*100), 0)
        #print(self.x,self.y)


    def inverse_kinematics(self):
        self.LL6=self.l6+self.h6+self.k4
        self.LL4=self.l4+self.h4
        print(f"LL4: {self.LL4} LL6: {self.LL6}")
        q4=(math.acos(((self.x*self.x)+(self.y*self.y)-(self.LL6*self.LL6)-(self.LL4*self.LL4))/(2*self.LL6*self.LL4)))
        q6=(math.atan(self.y/self.x))-(math.atan((self.LL4*math.sin(q4))/(self.LL6+(self.LL4*math.cos(q4)))))
        q4=math.degrees(q4)
        q6=math.degrees(q6)
        print(f"x: {self.x} y: {self.y}")
        print(f"q4: {q4} q6: {q6}")
        m=180-(abs(q6))-(self.f)
        m=math.radians(m)
        n=180-abs(q4)
        n=math.radians(n)

        #forward kinematics verification begins
        q4=math.radians(q4)
        q6=math.radians(q6)
        xfwd=(self.LL6*(math.cos(q6)))+((self.LL4)*(math.cos(abs(q4)-abs(q6))))
        yfwd=-(self.LL6*(math.sin(abs(q6))))+((self.LL4)*(math.sin(abs(q4)-abs(q6))))
        print(f"xfwd: {xfwd} yfwd: {yfwd}")
        #forward kinematics verification ends

        self.a6=math.sqrt((self.l6*self.l6)+(self.k6*self.k6)-((math.cos(m))*2*self.l6*self.k6))
        self.a4=math.sqrt((self.l4*self.l4)+(self.k4*self.k4)-((math.cos(n))*2*self.k4*self.l4))
        print(f"a4: {self.a4} a6: {self.a6}")
    



if(__name__ == "__main__"):
    rospy.init_node("test")
    kin = Kin()
   
    rate = rospy.Rate(10)
    while not rospy.is_shutdown():
        kin.pub.publish(((kin.a4)*100),((kin.a6)*100),0)
        rate.sleep()
    rospy.spin()
