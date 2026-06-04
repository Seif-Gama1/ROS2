#include "rclcpp/rclcpp.hpp"
#include "nav_msgs/msg/odometry.hpp"

class OdomPublisher : public rclcpp::Node
{
public:
    OdomPublisher()
        : Node("odom_publisher"),
          x_position_(0.0)
    {
        publisher_ =
            this->create_publisher<nav_msgs::msg::Odometry>(
                "/odom",
                10);

        timer_ =
            this->create_wall_timer(
                std::chrono::milliseconds(100),
                std::bind(&OdomPublisher::timer_callback, this));
    }

private:
    void timer_callback()
    {
        nav_msgs::msg::Odometry msg;

        x_position_ += 0.1;

        msg.header.stamp = this->now();
        msg.header.frame_id = "odom";

        msg.child_frame_id = "base_link";

        msg.pose.pose.position.x = x_position_;
        msg.pose.pose.position.y = 0.0;
        msg.pose.pose.position.z = 0.0;

        msg.pose.pose.orientation.x = 0.0;
        msg.pose.pose.orientation.y = 0.0;
        msg.pose.pose.orientation.z = 0.0;
        msg.pose.pose.orientation.w = 1.0;

        msg.twist.twist.linear.x = 1.0;
        msg.twist.twist.linear.y = 0.0;
        msg.twist.twist.linear.z = 0.0;

        msg.twist.twist.angular.x = 0.0;
        msg.twist.twist.angular.y = 0.0;
        msg.twist.twist.angular.z = 0.0;

        publisher_->publish(msg);

        RCLCPP_INFO(this->get_logger(), "Published x = %.2f", x_position_);
    }

    double x_position_;

    rclcpp::Publisher<nav_msgs::msg::Odometry>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
};


int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<OdomPublisher>());
    rclcpp::shutdown();

    return 0;
}