/*
 * ----- LEG LAYOUT -----
 *
 *       ___________
 *    3 |           | 0
 *      |     x     |
 *      |     ^     |
 *      |     |     |
 *    1 |y <- O     | 4
 *      |           |
 *      |           |
 *      |           |
 *    5 |___________| 2
 *
 */

#include "hex_control/Leg.h"
#include "hex_control/vec3d.h"
#include <vector>
#include <ros/ros.h>
#include <geometry_msgs/Vector3.h>
#include <std_msgs/Float64MultiArray.h>
#include <std_msgs/MultiArrayLayout.h>

std_msgs::Float64MultiArray cmd;
int angle[3] = {90, 90, 90};
// Leg legGroupA[3];//, legGroupB[3];
std::vector<Leg> legGroupA = {Leg(), Leg(), Leg()};
std::vector<Leg> legGroupB = {Leg(), Leg(), Leg()};
ros::Publisher joints_pub;

std::vector<vec3d> legPositionsA =
{
  vec3d(203, -119, 0),
  vec3d(0, 119, 0),
  vec3d(-203, -119, 0),
};
float legAnglesA[] = {-90,90,-90};

std::vector<vec3d> legPositionsB =
{
  vec3d(203, 119, 0),
  vec3d(0, -119, 0),
  vec3d(-203, 119, 0),
};
float legAnglesB[] = {90,-90,90};

float legMaxRadius = 0.1;

void setServos()
{

}

vec3d findStepCenter(vec3d pos, vec3d target, float stepHeight)
{
  vec3d midpoint = (target + pos) / 2;
  float centerDepth = 0.5; //REPLACE LATER
  midpoint.z -= centerDepth;
  return midpoint;
}

void target_cb(const geometry_msgs::Vector3ConstPtr& msg)
{
  cmd.layout.data_offset = 0;
  cmd.layout.dim[0].label = "cmd";
  cmd.layout.dim[0].size = 18;
  cmd.layout.dim[0].stride = 1;
  vec3d targetVec(msg->x*legMaxRadius, msg->y*legMaxRadius, 0);
  for(int i = 0; i < 3; i++)
  {
    legGroupA[i].setInterp(legGroupA[i].getHome() + targetVec, 0.2);
    legGroupB[i].setInterp(legGroupB[i].getHome() + targetVec, 0.2);
  }
  std::vector<vec3d> angles;
  for(int i = 0; i < 3; i++)
    angles.push_back(legGroupA[i].setServos());
  for(int i = 0; i < 3; i++)
    angles.push_back(legGroupB[i].setServos());
  for(int j = 0; j < angles.size(); j++)
  {
    for(int i = 0; i < 3; i++)
    {
      cmd.data.push_back(angles[j].n[i]);
      //      servo[j][i].write(90);
    }
  }
  joints_pub.publish(cmd);
  // setServos();
}

int main(int argc, char *argv[]) {
  for(int i=0;i<3;i++)
  {
    legGroupA[i].setLegOrigin(legPositionsA[i], legAnglesA[i]);
    legGroupB[i].setLegOrigin(legPositionsB[i], legAnglesB[i]);
  }

  ros::init(argc, argv, "hex_control");
  ros::NodeHandle nh;
  ros::Rate rate(25);
  ros::Subscriber target_sub = nh.subscribe<geometry_msgs::Vector3>("body_frame_target", 10, target_cb);
  joints_pub = nh.advertise<std_msgs::Float64MultiArray>("walker/joints_controller/command", 10);

  while(ros::ok())
  {
    ros::spinOnce();
    rate.sleep();
  }

  return 0;
}
