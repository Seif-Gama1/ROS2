#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"


class VelocityLimiter : public rclcpp::Node {
public:
    VelocityLimiter() : Node("vel_limiter")
    {
        subscriber_ = this->create_subscription<geometry_msgs::msg::Twist>(
            "/cmd_vel",
            rclcpp::QoS(10).best_effort(),
            std::bind(&VelocityLimiter::Twist_callback, this, std::placeholders::_1));

        publisher_ = this->create_publisher<geometry_msgs::msg::Twist>(
            "/cmd_vel_limited",
            10);
    }

private:
    void Twist_callback(const geometry_msgs::msg::Twist & msg)
    {
        geometry_msgs::msg::Twist limited_msg = msg;

        double lin = msg.linear.x;
        double ang = msg.angular.z;
        RCLCPP_INFO(this->get_logger(), "Code Reached");
        if (lin > 1.0){
            RCLCPP_WARN(this->get_logger(), "Limiting linear speed to 1m/s, Speed was: %.2fm/s", lin);
            limited_msg.linear.x = 1.0;
        }

        if (ang > 1.5 || ang<-1.5){
            RCLCPP_WARN(this->get_logger(), "Limiting angular speed to 1.5rad/s, Speed was: %.2frad/s", ang);
            limited_msg.angular.z = 1.5;
        }
        publisher_->publish(limited_msg);
    }

    rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr subscriber_;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<VelocityLimiter>());
    rclcpp::shutdown();
    return 0;
}