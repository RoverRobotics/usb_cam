#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <move_base_msgs/MoveBaseGoal.h>
#include <actionlib/client/simple_action_client.h>
#include "usb_cam/HandleTopic.h"


typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

int main(int argc, char** argv){

  ros::init(argc, argv, "simple_nav_and_dock");

  ros::NodeHandle n;
  ros::ServiceClient client = n.serviceClient<usb_cam::HandleTopic>("start_pub");
  usb_cam::HandleTopic srv;
  srv.request.topic_name = "/camera";
  
  //tell the action client that we want to spin a thread by default
  MoveBaseClient ac("move_base", true);

  //wait for the action server to come up
  while(!ac.waitForServer(ros::Duration(5.0))){
    ROS_INFO("Waiting for the move_base action server to come up");
  }

  move_base_msgs::MoveBaseGoal goal;

  //we'll send a goal to the robot to move 1 meter forward
  goal.target_pose.header.frame_id = "base_link";
  goal.target_pose.header.stamp = ros::Time::now();

  goal.target_pose.pose.position.x = 91.191;
  goal.target_pose.pose.position.y =  28.113;
  goal.target_pose.pose.position.z = 0;
  goal.target_pose.pose.orientation.x = 0.;
  goal.target_pose.pose.orientation.y = 0;
  goal.target_pose.pose.orientation.z = 1.0;
  goal.target_pose.pose.orientation.w = -0.015;

  ROS_INFO("Sending goal");
  ac.sendGoal(goal);

  ac.waitForResult();

  if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
  {
    ROS_INFO("Hooray");
    if (client.call(srv))
    {
      ROS_INFO("Creating new image pub to topic %s", "/camera");
    }
    else
    {
      ROS_ERROR("Failed to setup new image topic.");
    }
  }
  else
  {
    ROS_INFO("The base failed to move to desired location");
  }

  return 0;
}