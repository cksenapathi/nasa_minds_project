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

#include "Leg.h"
// #include <Servo.h>
#include "vec3d.h"
#include <vector>
#include <ros/ros.h>
#include <geometry_msgs/Vector3.h>
#include <std_msgs/Float64MultiArray.h>
#include <std_msgs/MultiArrayLayout.h>
// #include "RadioInput.h"

// Servo servo[3][3];
// int servoPins[] = {3,4,5,6,7,8,9,10,11};
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
  Float64MultiArray cmd;
  vec3d targetVec(msg->x*legMaxRadius, msg->y*legMaxRadius, 0);
  for(int i = 0; i < 3; i++)
  {
    legGroupA[i].setInterp(legGroupA[i].getHome() + targetVec, 0.2);
  }
  std::vector<vec3d> angles;
  vec3d angles[3];
  for(int i = 0; i < 3; i++)
  angles[i] = legGroupA[i].setServos();
  for(int j = 0; j < 3; j++)
  {
    for(int i = 0; i < 3; i++)
    {
      servo[j][i].write(angles[j].n[i]);
      //      servo[j][i].write(90);
    }
  }
  // setServos();
}

int main(int argc, char const *argv[]) {
  for(int i=0;i<3;i++)
  {
    legGroupA[i].setLegOrigin(legPositionsA[i], legAnglesA[i]);
    legGroupB[i].setLegOrigin(legPositionsB[i], legAnglesB[i]);
  }
  return 0;

  ros::init(argc, argv, "hex_control");
  ros::NodeHandle nh;
  ros::Rate rate(25);
  ros::Subscriber target_sub = nh.subscribe<geometry_msgs::Vector3("body_frame_target")
  joints_pub = nh.advertise<

}

void setup() {
  // for(int i = 0; i < 9; i++)
  //   servo[i/3][i%3].attach(servoPins[i]);

  for(int i = 0; i < 3; i++)
  {
    legGroupA[i].setLegOrigin(legPositionsA[i], legAnglesA[i]);
   legGroupB[i].setLegOrigin(legPositionsB[i], legAnglesB[i]);
  }
  remote.begin();
  Serial.begin(9600);
}

void loop() {
  remote.update();
  remote.getTranslation().println();


  vec3d move = remote.getTranslation();
  move.z = 0;

  vec3d targetVec = move* legMaxRadius;


  for(int i = 0; i < 3; i++)
  {
    legGroupA[i].setInterp(legGroupA[i].getHome() + targetVec, 0.2);
  }

  setServos();
  delay(20);

}

//    vec3d angles[3];
//    for(int i = 0; i < 3; i++)
//      angles[i] = legGroupA[i].setServos();
//    for(int j = 0; j < 3; j++)
//    {
//      for(int i = 0; i < 3; i++)
//      {
//        servo[j][i].write(angles[j].n[i]);
//  //      servo[j][i].write(90);
//      }
//    }



//  switch(Serial.read())
//  {
//    case 'w':
//    leg.translateLocalSpace(vec3d::X * 0.01);
//    break;
//
//    case 's':
//    leg.translateLocalSpace(vec3d::X * 0.01* -1);;
//    break;
//
//    case 'd':
//    leg.translateLocalSpace(vec3d::Y * 0.01);
//    break;
//
//    case 'a':
//    leg.translateLocalSpace(vec3d::Y * 0.01 * -1);;
//    break;
//
//    case 'e':
//    leg.translateLocalSpace(vec3d::Z * 0.01);
//    break;
//
//    case 'q':
//    leg.translateLocalSpace(vec3d::Z * 0.01 * -1);;
//    break;
//
//
//  }
