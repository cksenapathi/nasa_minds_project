#include <ros/ros.h>
#include <pcl_ros/point_cloud.h>
#include <pcl_ros/transforms.h>
#include <pcl/point_types.h>
#include <pcl/conversions.h>
#include <sensor_msgs/PointField.h>
#include <sensor_msgs/PointCloud2.h>
#include <tf/transform_listener.h>

typedef pcl::PointCloud<pcl::PointXYZRGBA> PointCloud;

PointCloud full_pc;
sensor_msgs::PointField field;
sensor_msgs::PointCloud2 full_pc2;
// PointCloud r_pc;
// PointCloud l_pc;
// PointCloud m_pc;
pcl::PCLPointCloud2 r_pc2;
pcl::PCLPointCloud2 l_pc2;
pcl::PCLPointCloud2 m_pc2;

// gather all the point clouds and add them together
// they are already in cam_ml frame so they can just be added HOLD ON THEY'RE IN BAD FRAMES EACH NEEDS ITS OWN TRANSFORM listener
// convert to body frame through http://docs.ros.org/en/api/pcl_ros/html/namespacepcl__ros.html#a3f2ac4f97e01edef0e546d146af6afa2
//    transform point cloud --> requires types to be pcl::PointCloud type, and conversion can be taken care of by pcl
// once in body frame, could potentially need transformation from vertical to horizontal
// publish to new topic, which goes to rtabmap, still requires odometry--> position estimate


void mid_cb(const sensor_msgs::PointCloud2ConstPtr& msg)
{
  // printf("Cloud size: width: %d height: %d\n", msg->width, msg->height);
  // printf("is_bigendian %d is_dense %d point step %d row_step %d\n", msg->is_bigendian, msg->is_dense, msg->point_step, msg->row_step);
  // field = msg->fields[0];
  // printf("Name: %s, offset: %d, datatype: %d, count: %d\n", field.name.c_str(), field.offset, field.datatype, field.count);
  // printf("Middle Scan Frame: %s Time: %d\n", msg->header.frame_id.c_str(), msg->header.stamp.sec);
  tf::StampedTransform mid_to_body;
  sensor_msgs::PointCloud2 tf_pc;
  tf::TransformListener mtb_listener;
  try{
    mtb_listener.waitForTransform("body", "cam_ll", ros::Time(0), ros::Duration(10));
    mtb_listener.lookupTransform("body", "cam_ll", ros::Time(0), mid_to_body);
  }
  catch(tf::TransformException &ex){
    ROS_ERROR("%s",ex.what());
    ros::Duration(1.0).sleep();
  }
  pcl_ros::transformPointCloud("body", *msg, tf_pc, mtb_listener);
  pcl::concatenatePointCloud(full_pc2, tf_pc, full_pc2);
}

void left_cb(const sensor_msgs::PointCloud2ConstPtr& msg)
{
  // printf("Cloud size: width: %d height: %d\n", msg->width, msg->height);
  // printf("is_bigendian %d is_dense %d point step %d row_step %d\n", msg->is_bigendian, msg->is_dense, msg->point_step, msg->row_step);
  // field = msg->fields[0];
  // printf("Name: %s, offset: %d, datatype: %d, count: %d\n", field.name.c_str(), field.offset, field.datatype, field.count);
  // printf("Left Scan Frame: %s Length of fields: %d\n", msg->header.frame_id.c_str(), msg->fields.size());
  tf::StampedTransform left_to_body;
  tf::TransformListener ltb_listener;
  // for(int i=0;i<msg->fields.size();i++)
  // {
  //   field = msg->fields[i];
  //   printf("Field Name %s, datatype: %d, count: %d \n",field.name.c_str(), field.datatype, field.count);
  // }
  sensor_msgs::PointCloud2 tf_pc;
  try{
    ltb_listener.waitForTransform("body", "cam_ll", ros::Time(0), ros::Duration(10));
    ltb_listener.lookupTransform("body", "cam_ll", ros::Time(0), left_to_body);
  }
  catch(tf::TransformException &ex){
    ROS_ERROR("%s",ex.what());
    ros::Duration(1.0).sleep();
  }
  pcl_ros::transformPointCloud("body", *msg, tf_pc, ltb_listener);
  pcl::concatenatePointCloud(full_pc2, tf_pc, full_pc2);
}

void right_cb(const sensor_msgs::PointCloud2ConstPtr& msg)
{
  tf::StampedTransform right_to_body;
  tf::TransformListener rtb_listener;
  try{
    rtb_listener.waitForTransform("body", "cam_ll", ros::Time(0), ros::Duration(10));
    rtb_listener.lookupTransform("body", "cam_ll", ros::Time(0), right_to_body);
  }
  catch(tf::TransformException &ex){
    ROS_ERROR("%s",ex.what());
    ros::Duration(1.0).sleep();
  }
  // printf("Cloud size: width: %d height: %d\n", msg->width, msg->height);
  // printf("is_bigendian %d is_dense %d point step %d row_step %d\n", msg->is_bigendian, msg->is_dense, msg->point_step, msg->row_step);
  // printf("Name: %s, offset: %d, datatype: %d, count: %d\n", field.name.c_str(), field.offset, field.datatype, field.count);
  // field = msg->fields[0];
  // printf("Right Scan Frame: %s Time: %d\n", msg->header.frame_id.c_str(), msg->header.stamp.sec);
  sensor_msgs::PointCloud2 tf_pc;
  pcl_ros::transformPointCloud("body", *msg, tf_pc, rtb_listener);
  printf("Cloud size: width: %d height: %d\n", tf_pc.width, tf_pc.height);
  printf("is_bigendian %d is_dense %d point step %d row_step %d\n", tf_pc.is_bigendian, tf_pc.is_dense, tf_pc.point_step, tf_pc.row_step);
  pcl::concatenatePointCloud(full_pc2, tf_pc, full_pc2);
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "sub_pcl");
  ros::NodeHandle nh;
  ros::Rate rate(10);
  ros::Subscriber sub_m = nh.subscribe<sensor_msgs::PointCloud2>("walker/mid/points2", 1, mid_cb);
  ros::Subscriber sub_l = nh.subscribe<sensor_msgs::PointCloud2>("walker/left/points2", 1, left_cb);
  ros::Subscriber sub_r = nh.subscribe<sensor_msgs::PointCloud2>("walker/right/points2", 1, right_cb);
  ros::Publisher pc_pub = nh.advertise<sensor_msgs::PointCloud2>("walker/points2", 25);
  while(nh.ok())
  {
    // full_pc = m_pc; // + r_pc;
    // full_pc += l_pc;
    // pcl::toROSMsg(full_pc, full_pc2);

    printf("Main Cloud size: width: %d height: %d\n", full_pc2.width, full_pc2.height);
    full_pc2.header.frame_id = "body";
    full_pc2.header.stamp = ros::Time::now();
    tf::Transform transform;
    transform.setOrigin(tf::Vector3(0, 0, 0.0));
    tf::Quaternion q;
    q.setRPY(0, 1.570796, -3.141592);
    transform.setRotation(q);
    pcl_ros::transformPointCloud("body", transform, full_pc2, full_pc2);
    pc_pub.publish(full_pc2);
    full_pc2.height = 0;
    full_pc2.width = 0;
    full_pc2.fields = {};
    full_pc2.point_step = 0;
    full_pc2.row_step = 0;
    full_pc2.data = {};

    ros::spinOnce();
    rate.sleep();
  }
  // ros::spin();
}
