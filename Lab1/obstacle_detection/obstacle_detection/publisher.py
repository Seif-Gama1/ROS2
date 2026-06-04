import rclpy
from rclpy.node import Node

from std_msgs.msg import Float32, Bool
from random import uniform

class MinimalPublisher(Node):

    def __init__(self):
        super().__init__('random_n_publisher')
        self.publisher_ = self.create_publisher(Float32, 'sensor/distance', 10)
        timer_period = 0.1  # seconds

        self.subscription = self.create_subscription(
            Bool,
            'cmd/stop',
            self.listener_callback,
            10)

        self.timer = self.create_timer(timer_period, self.timer_callback)

    def timer_callback(self):
        msg = Float32()
        msg.data = uniform(0.03, 5.0)
        self.publisher_.publish(msg)
        self.get_logger().info(f"Publishing: {msg.data:.2f}")

    def listener_callback(self, msg: Bool):
        self.get_logger().info(f"Publisher Received: {msg.data}")

def main(args=None):
    rclpy.init(args=args)

    minimal_publisher = MinimalPublisher()

    rclpy.spin(minimal_publisher)

    # Destroy the node explicitly
    # (optional - otherwise it will be done automatically
    # when the garbage collector destroys the node object)
    minimal_publisher.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()