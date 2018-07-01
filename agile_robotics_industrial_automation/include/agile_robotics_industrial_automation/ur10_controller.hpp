// BSD 3-Clause License

// Copyright (c) 2018, Neel Parikh
// All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.

// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.

// * Neither the name of the copyright holder nor the names of its
//   contributors may be used to endorse or promote products derived from
//   this software without specific prior written permission.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#pragma once

#include <stdarg.h>
#include <initializer_list>
#include <iostream>
#include <string>

#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit/planning_scene_interface/planning_scene_interface.h>
#include <moveit/robot_model_loader/robot_model_loader.h>
#include <ros/ros.h>
#include <tf/transform_listener.h>

#include <control_msgs/JointTrajectoryControllerState.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/PoseStamped.h>
#include <trajectory_msgs/JointTrajectoryPoint.h>

#include <osrf_gear/LogicalCameraImage.h>
#include <osrf_gear/VacuumGripperControl.h>
#include <osrf_gear/VacuumGripperState.h>
#include <osrf_gear/Proximity.h>

#include "agile_robotics_industrial_automation/sensor.hpp"

class UR10Controller {
 public:
  UR10Controller();
  ~UR10Controller();
  bool planner();
  void execute();
  void goToTarget(std::initializer_list<geometry_msgs::Pose> list);
  void goToTarget(const geometry_msgs::Pose& pose);
  void sendRobot(std::vector<double> tar, double duration=1);
  bool dropPart(geometry_msgs::Pose pose);
  void gripperToggle(const bool& state);
  void gripperCallback(const osrf_gear::VacuumGripperState::ConstPtr& grip);
  void jointCallback1(const sensor_msgs::JointState::ConstPtr& joint);
  void jointCallback2(
      const control_msgs::JointTrajectoryControllerState::ConstPtr& joint_msg);
  void quality1Callback(
      const osrf_gear::LogicalCameraImage::ConstPtr& quality1_);
  void quality2Callback(
      const osrf_gear::LogicalCameraImage::ConstPtr& quality2_);
  void breakBeamCallback(const osrf_gear::Proximity::ConstPtr& msg);
  void convCallback(const osrf_gear::LogicalCameraImage::ConstPtr& conv_msg);  
  void gripper_state_check(geometry_msgs::Pose pose);
  bool pickPart(geometry_msgs::Pose& part_pose);
  void publishJoints(const std::vector<double>& tar, double duration = 1.0);
  bool goToConveyor(const geometry_msgs::Pose& part_pose, std::string type);
  bool flipPart(geometry_msgs::Pose& part_pose);
  bool pickPartpulley(geometry_msgs::Pose& part_pose);

 private:
  ros::NodeHandle ur10_nh_;
  ros::ServiceClient gripper_client_;
  ros::NodeHandle gripper_nh_;
  ros::Subscriber gripper_subscriber_;
  ros::Subscriber camera_1_subscriber_;
  ros::Subscriber joint_subscriber_;
  ros::Publisher joint_publisher_;
  ros::Subscriber arm_state_subscriber_;
  ros::Subscriber quality1_subscriber_;
  ros::Subscriber quality2_subscriber_;
  ros::Subscriber break_beam_subscriber_;
  ros::Subscriber conv_subscriber_;

  tf::TransformListener robot_tf_listener_;
  tf::StampedTransform robot_tf_transform_;
  tf::TransformListener agv_tf_listener_;
  tf::StampedTransform agv_tf_transform_;

  moveit::planning_interface::MoveGroupInterface robot_move_group_;
  moveit::planning_interface::MoveGroupInterface::Plan robot_planner_;

  osrf_gear::VacuumGripperControl gripper_service_;
  osrf_gear::VacuumGripperState gripper_status_;
  osrf_gear::LogicalCameraImage parts_on_conv_;  

  geometry_msgs::Pose target_pose_;
  geometry_msgs::Pose home_cart_pose_;
  geometry_msgs::Quaternion fixed_orientation_;
  geometry_msgs::Pose agv_position_;
  control_msgs::JointTrajectoryControllerState curr_joint_states_;
  geometry_msgs::PoseStamped conv_pose_;
  sensor_msgs::JointState joint_state_;

  std::string object;
  bool plan_success_;
  std::vector<double> home_joint_pose_, conv_joint_pose_, temp1, temp2;
  std::vector<double> end_position_;
  double offset_, roll_def_, pitch_def_, yaw_def_;
  tf::Quaternion q;
  int counter_;
  bool gripper_state_, drop_flag_, part_detected_;
  osrf_gear::LogicalCameraImage quality_1, quality_2;
  int alternate=1;

  Sensor conv_camera_;
  geometry_msgs::Pose static_pose_;
};