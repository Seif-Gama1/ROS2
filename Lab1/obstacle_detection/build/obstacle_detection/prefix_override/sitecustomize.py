import sys
if sys.prefix == '/usr':
    sys.real_prefix = sys.prefix
    sys.prefix = sys.exec_prefix = '/home/seif/workspace/my_ros/new_dir/obstacle_detection/install/obstacle_detection'
