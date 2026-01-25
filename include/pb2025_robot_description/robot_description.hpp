// Copyright 2025 SMBU-PolarBear-Robotics-Team
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef PB2025__ROBOT_DESCRIPTION__ROBOT_DESCRIPTION_HPP_
#define PB2025__ROBOT_DESCRIPTION__ROBOT_DESCRIPTION_HPP_

#include <string>
#include <memory>

#include "rclcpp/rclcpp.hpp"

#include "sensor_msgs/msg/imu.hpp"
#include "sensor_msgs/msg/joint_state.hpp"
#include "tf2_ros/transform_broadcaster.h"

namespace robot_description
{
class RobotDescription : public rclcpp::Node
{
public:
  explicit RobotDescription(const rclcpp::NodeOptions & options);

  ~RobotDescription() override;

private:

  // std::string base_yaw_imu_frame_;
  // std::string base_yaw_frame_;

  rclcpp::Publisher<sensor_msgs::msg::JointState>::SharedPtr joint_state_pub_;

  rclcpp::Subscription<sensor_msgs::msg::Imu>::SharedPtr livox_imu_sub_;

  void LivoxImuCallback(const sensor_msgs::msg::Imu::SharedPtr msg);

  std::unique_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;

};
}  // namespace robot_description

#endif  // PB2025__ROBOT_DESCRIPTION__ROBOT_DESCRIPTION_HPP_
