#include "pb2025_robot_description/robot_description.hpp"

#include "tf2/LinearMath/Quaternion.h"
#include "tf2/LinearMath/Matrix3x3.h"

namespace robot_description
{

RobotDescription::RobotDescription(const rclcpp::NodeOptions & options)
: Node("robot_description", options)
{
  RCLCPP_INFO(get_logger(), "Start RobotDescription!");

  // this->declare_parameter<std::string>("base_yaw_imu_frame", "livox_imu");
  // this->declare_parameter<std::string>("base_yaw_frame", "gimbal_yaw");

  // this->get_parameter("base_yaw_imu_frame", base_yaw_imu_frame_);
  // this->get_parameter("base_yaw_frame", base_yaw_frame_);

  tf_broadcaster_ = std::make_unique<tf2_ros::TransformBroadcaster>(*this);

  joint_state_pub_ =
    this->create_publisher<sensor_msgs::msg::JointState>("gimbal_joint_state", 1);

  livox_imu_sub_ = this->create_subscription<sensor_msgs::msg::Imu>(
    "imu/gimbal_yaw", 10,
    std::bind(&RobotDescription::LivoxImuCallback, this, std::placeholders::_1));

}

void RobotDescription::LivoxImuCallback(const sensor_msgs::msg::Imu::SharedPtr msg)
{
  tf2::Quaternion q(
    msg->orientation.x, msg->orientation.y, msg->orientation.z, msg->orientation.w);
  tf2::Matrix3x3 m(q);
  double roll, pitch, yaw;
  m.getRPY(roll, pitch, yaw);
 
  // RCLCPP_WARN(get_logger(),"gimbal_yaw_joint: %f",yaw);
  // RCLCPP_WARN(get_logger(),"gimbal_yaw_joint: %f",packet.data.yaw);

  sensor_msgs::msg::JointState joint_msg;

  joint_msg.name = {
    "gimbal_yaw_joint",
    "gimbal_yaw_odom_joint",
  };

  joint_msg.position = {
    yaw,
    0.0,
  };

  joint_state_pub_->publish(joint_msg);
}

}  // namespace robot_description

#include "rclcpp_components/register_node_macro.hpp"
RCLCPP_COMPONENTS_REGISTER_NODE(robot_description::RobotDescription)
