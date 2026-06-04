import rclpy
from rclpy.node import Node

from std_msgs.msg import Float32, Bool


class MinimalSubscriber(Node):

    def __init__(self):
        super().__init__('minimal_subscriber')
        self.subscription = self.create_subscription(
            Float32,
            'sensor/distance',
            self.listener_callback,
            10)
        
        self.publisher_ = self.create_publisher(Bool, 'cmd/stop', 10)
        
        self.subscription  # prevent unused variable warning


    def listener_callback(self, msg: Float32):
        stop_msg = Bool()

        if msg.data < 2.0:
            stop_msg.data = True
        else:
            stop_msg.data = False

        self.publisher_.publish(stop_msg)

        self.get_logger().info(
            f"Distance: {msg.data:.2f} -> STOP: {stop_msg.data}"
        )

def main(args=None):
    rclpy.init(args=args)

    minimal_subscriber = MinimalSubscriber()

    rclpy.spin(minimal_subscriber)

    # Destroy the node explicitly
    # (optional - otherwise it will be done automatically
    # when the garbage collector destroys the node object)
    minimal_subscriber.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()