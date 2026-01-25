# Copyright 2025 SMBU-PolarBear-Robotics-Team
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import os

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import (
    DeclareLaunchArgument,
    GroupAction,
    IncludeLaunchDescription,
    SetEnvironmentVariable,
)
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node, PushRosNamespace, SetRemap
from launch_ros.descriptions import ParameterFile
from nav2_common.launch import RewrittenYaml


def generate_launch_description():

    stdout_linebuf_envvar = SetEnvironmentVariable(
        "RCUTILS_LOGGING_BUFFERED_STREAM", "1"
    )

    colorized_output_envvar = SetEnvironmentVariable("RCUTILS_COLORIZED_OUTPUT", "1")

    # Specify the actions
    bringup_cmd_group = GroupAction(
        [
            SetRemap("/tf", "tf"),
            SetRemap("/tf_static", "tf_static"),
            Node(
                package='imu_transformer',
                executable='imu_transformer_node',
                name='imu_transformer_node',
                parameters=[{
                    'target_frame': 'base_yaw'
                }],
                remappings=[
                    ('/imu_in', '/livox/imu'),
                    ('/imu_out', '/imu/transformed'),
                ]
            ),
            Node(
                package='imu_filter_madgwick',
                executable='imu_filter_madgwick_node',
                name='imu_filter_madgwick_node',
                parameters=[{
                    'use_mag': False,
                    'world_frame': 'enu',
                    'fixed_frame': 'base_yaw_odom',
                    'imu_frame': 'base_yaw',
                    'gain': 0.1,
                    'zeta': 0.0,
                    'publish_tf': True,
                    'publish_debug_topics': False,
                    # 重力参数
                    'gravity_constant': 9.81,
                    'remove_gravity_acceleration': False, 
                    # 初始化参数
                    'orientation_stddev': 0.0,
                    'angular_velocity_stddev': 0.02,
                    'linear_acceleration_stddev': 0.04,
                    # 静态检测参数
                    'stat': {
                        'stationary_time': 5.0,
                        'acceleration_threshold': 0.1,
                        'magnetic_threshold': 0.0
                    },
                    'frequency': 200.0,
                    'constant_dt': 0.0
                    }],
                remappings=[
                    ('/imu/data_raw', '/imu/transformed'),
                    ('/imu/data', '/imu/base_yaw')
                ]
            ),
        ]
    )

    # Create the launch description and populate
    ld = LaunchDescription()

    # Set environment variables
    ld.add_action(stdout_linebuf_envvar)
    ld.add_action(colorized_output_envvar)

    # Add the actions to launch all of nodes
    ld.add_action(bringup_cmd_group)

    return ld
