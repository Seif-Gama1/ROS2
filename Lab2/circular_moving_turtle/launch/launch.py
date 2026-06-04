# launch/lap_degree.launch.py
from launch import LaunchDescription
from launch_ros.actions import Node
import os 
from ament_index_python.packages import get_package_share_directory

def generate_launch_description():
    
    params_file = os.path.join(
        get_package_share_directory('circular_moving_turtle'),
        'params',
        'patrol_params.yaml'
    )

    return LaunchDescription([
        Node(
            package='circular_moving_turtle',
            executable='patrol_controller',
            name='patrol_controller',
            output='screen',
            parameters=[params_file]
        ),
        Node(
            package='circular_moving_turtle',
            executable='status_publisher',
            name='status_publisher',
            output='screen',
            parameters=[params_file]
        ),
        Node(
            package='turtlesim',
            executable='turtlesim_node',
            name='turtlesim_node',
            output='screen',
        )
    ])