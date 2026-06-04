import rclpy
from rclpy.node import Node
from sensor_msgs.msg import Imu
from ament_index_python.packages import get_package_share_directory
import os
import csv

class ImuCsvPlayback(Node):

    def __init__(self):
        super().__init__('imu_csv_playback')
        self.publisher_ = self.create_publisher(Imu, '/imu/data', 10)
        timer_period = 0.1  # seconds
        self.timer = self.create_timer(timer_period, self.timer_callback)


        csv_path = os.path.join(
            get_package_share_directory('imu_csv_playback'),
            'data',
            'imu_data.csv'
        )

        with open(csv_path, 'r') as csvfile:
            self.rows = list(csv.DictReader(csvfile))

        self.current_index = 0

    def timer_callback(self):
        row = self.rows[self.current_index]
        self.get_logger().debug(str(row))

        msg = Imu()
        msg.header.stamp = self.get_clock().now().to_msg()
        msg.header.frame_id = "base_link"

        msg.orientation.w = float(row["orient_w"])
        msg.orientation.x = float(row["orient_x"])
        msg.orientation.y = float(row["orient_y"])
        msg.orientation.z = float(row["orient_z"])

        msg.angular_velocity.x = float(row["ang_x"])
        msg.angular_velocity.y = float(row["ang_y"])
        msg.angular_velocity.z = float(row["ang_z"])

        msg.linear_acceleration.x = float(row["acc_x"])
        msg.linear_acceleration.y = float(row["acc_y"])
        msg.linear_acceleration.z = float(row["acc_z"])

        msg.orientation_covariance = [0.0] * 9
        msg.angular_velocity_covariance = [0.0] * 9
        msg.linear_acceleration_covariance = [0.0] * 9
        
        self.publisher_.publish(msg)
        
        self.get_logger().info(f"IMU: ang_z={msg.angular_velocity.z:.2f}, acc_z={msg.linear_acceleration.z:.2f}")

        self.current_index = (self.current_index + 1) % len(self.rows)
    
def main(args=None):
    rclpy.init(args=args)

    imu_csv_playback = ImuCsvPlayback()

    rclpy.spin(imu_csv_playback)

    # Destroy the node explicitly
    # (optional - otherwise it will be done automatically
    # when the garbage collector destroys the node object)
    imu_csv_playback.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()