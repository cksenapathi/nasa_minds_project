#!/usr/bin/env python3

import rospy
from std_msgs.msg import Float64MultiArray, MultiArrayLayout, MultiArrayDimension
from sensor_msgs.msg import JointState

pub = rospy.Publisher('/walker/joints_controller/command', Float64MultiArray, queue_size=10)


def callback(data):
    msg = Float64MultiArray()
    # dim1 = MultiArrayDimension()
    # layout1 = MultiArrayLayout()
    # msg.layout.dim.label=""

    msg.data = data.position
    msg.layout.data_offset = 0
    msg.layout.dim = [MultiArrayDimension(label='dim1', size=2, stride=1), ]
    pub.publish(msg)


def main():
    rospy.init_node('joint_transfer_node', anonymous=True)
    rate = rospy.Rate(50)
    rospy.Subscriber('/joint_states', JointState, callback)
    rospy.spin()


if __name__ == '__main__':
    try:
        main()
    except rospy.ROSInterruptException as e:
        rospy.logwarn(e)
