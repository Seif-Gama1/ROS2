from launch import LaunchDescription
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare
from launch.substitutions import PathJoinSubstitution
import os
from ament_index_python.packages import get_package_share_directory

pkg_dir = get_package_share_directory('robot_tf_localization')

rviz_config = os.path.join( pkg_dir, 'config', 'robot_tf.rviz' )

def generate_launch_description():
    pkg_share = FindPackageShare('robot_tf_localization')

    ekf_config = PathJoinSubstitution([
        pkg_share,
        'config',
        'ekf.yaml'
    ])

    node = LaunchDescription([
        # base_footprint -> base_link
        Node(
            package='tf2_ros',
            executable='static_transform_publisher',
            arguments=[
                '--x', '0.0',
                '--y', '0.0',
                '--z', '0.05',
                '--roll', '0.0',
                '--pitch', '0.0',
                '--yaw', '0.0',
                '--frame-id', 'base_footprint',
                '--child-frame-id', 'base_link'
            ]
        ),
        # base_link -> imu_link
        Node(
            package='tf2_ros',
            executable='static_transform_publisher',
            arguments=[
                '--x', '0.50',
                '--y', '-0.10',
                '--z', '0.1',
                '--roll', '0.0',
                '--pitch', '0.0',
                '--yaw', '0.0',
                '--frame-id', 'base_link',
                '--child-frame-id', 'imu_link'
            ]
        ),
        # base_link -> gps_link
        Node(
            package='tf2_ros',
            executable='static_transform_publisher',
            arguments=[
                '--x', '0.2',
                '--y', '0.0',
                '--z', '0.25',
                '--roll', '0.0',
                '--pitch', '0.0',
                '--yaw', '0.0',
                '--frame-id', 'base_link',
                '--child-frame-id', 'gps_link'
            ]
        ),
        # base_link -> ultrasonic1_link (front-left)
        Node(
            package='tf2_ros',
            executable='static_transform_publisher',
            arguments=[
                '--x', '0.5',
                '--y', '0.15',
                '--z', '0.1',
                '--roll', '0.0',
                '--pitch', '0.0',
                '--yaw', '0.7854',  # 45 degrees in radians
                '--frame-id', 'base_link',
                '--child-frame-id', 'ultrasonic1_link'
            ]
        ),
        # base_link -> ultrasonic2_link (front_center)
        Node(
            package='tf2_ros',
            executable='static_transform_publisher',
            arguments=[
                '--x', '0.5',
                '--y', '0.0',
                '--z', '0.1',
                '--roll', '0.0',
                '--pitch', '0.0',
                '--yaw', '0.0',
                '--frame-id', 'base_link',
                '--child-frame-id', 'ultrasonic2_link'
            ]
        ),
        # base_link -> ultrasonic3_link (front_right)
        Node(
            package='tf2_ros',
            executable='static_transform_publisher',
            arguments=[
                '--x', '0.5',
                '--y', '-0.15',
                '--z', '0.1',
                '--roll', '0.0',
                '--pitch', '0.0',
                '--yaw', '-0.7854',  # -45 degrees in radians
                '--frame-id', 'base_link',
                '--child-frame-id', 'ultrasonic3_link'
            ]
        ),
        # base_link -> ultrasonic4_link (rear_left)
        Node(
            package='tf2_ros',
            executable='static_transform_publisher',
            arguments=[
                '--x', '-0.1',
                '--y', '0.15',
                '--z', '0.1',
                '--roll', '0.0',
                '--pitch', '0.0',
                '--yaw', '2.3562',  # 135 degrees in radians
                '--frame-id', 'base_link',
                '--child-frame-id', 'ultrasonic4_link'
            ]
        ),
        # base_link -> ultrasonic5_link (rear_center)
        Node(
            package='tf2_ros',
            executable='static_transform_publisher',
            arguments=[
                '--x', '-0.1',
                '--y', '0.0',
                '--z', '0.1',
                '--roll', '0.0',
                '--pitch', '0.0',
                '--yaw', '3.1416',  # 180 degrees in radians
                '--frame-id', 'base_link',
                '--child-frame-id', 'ultrasonic5_link'
            ]
        ),
        # base_link -> ultrasonic6_link (rear_right)
        Node(
            package='tf2_ros',
            executable='static_transform_publisher',
            arguments=[
                '--x', '-0.1',
                '--y', '-0.15',
                '--z', '0.1',
                '--roll', '0.0',
                '--pitch', '0.0',
                '--yaw', '-2.3562',  # -135 degrees in radians
                '--frame-id', 'base_link',
                '--child-frame-id', 'ultrasonic6_link'
            ]
        ),
        Node(
            package="rviz2",
            executable="rviz2",
            name='rviz2',
            output = 'screen',
        ),
        Node(
            package="robot_localization",
            executable="ekf_node",
            name="ekf_filter_node",
            output="screen",
            parameters=["config/ekf.yaml"],
            remappings=[
                ("odometry/filtered", "/odometry/local"),
            ],
        )
    ])

    return node