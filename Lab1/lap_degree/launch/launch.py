# launch/lap_degree.launch.py
from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(
            package='lap_degree',
            executable='temp_publisher',
            name='cpu_temp_publisher',
            output='screen',
        ),
        Node(
            package='lap_degree',
            executable='temp_subscriber',
            name='cpu_temp_subscriber',
            output='screen',
        ),
    ])