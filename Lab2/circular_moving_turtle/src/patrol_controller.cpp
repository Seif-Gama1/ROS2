#include <chrono>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "std_srvs/srv/empty.hpp"

using namespace std::chrono_literals;

class PatrolController : public rclcpp::Node
{
public:
    PatrolController()
    : Node("patrol_controller"),
      running_(true)
    {
        // Parameters
        this->declare_parameter("linear_speed", 1.5);
        this->declare_parameter("angular_speed", 1.0);

        publisher_ =
            this->create_publisher<geometry_msgs::msg::Twist>(
                "/turtle1/cmd_vel", 10);

        stop_service_ =
            this->create_service<std_srvs::srv::Empty>(
                "/stop",
                std::bind(
                    &PatrolController::stop_callback,
                    this,
                    std::placeholders::_1,
                    std::placeholders::_2));

        continue_service_ =
            this->create_service<std_srvs::srv::Empty>(
                "/continue",
                std::bind(
                    &PatrolController::continue_callback,
                    this,
                    std::placeholders::_1,
                    std::placeholders::_2));

        timer_ =
            this->create_wall_timer(
                100ms,
                std::bind(&PatrolController::timer_callback, this));

        RCLCPP_INFO(this->get_logger(), "Patrol Controller Started");
    }

private:
    void timer_callback()
    {
        auto msg = geometry_msgs::msg::Twist();

        double linear_speed =
            this->get_parameter("linear_speed").as_double();

        double angular_speed =
            this->get_parameter("angular_speed").as_double();

        if (running_)
        {
            msg.linear.x = linear_speed;
            msg.angular.z = angular_speed;
        }
        else
        {
            msg.linear.x = 0.0;
            msg.angular.z = 0.0;
        }

        publisher_->publish(msg);
    }

    void stop_callback(
        const std::shared_ptr<std_srvs::srv::Empty::Request>,
        std::shared_ptr<std_srvs::srv::Empty::Response>)
    {
        running_ = false;
        RCLCPP_INFO(this->get_logger(), "Patrol stopped");
    }

    void continue_callback(
        const std::shared_ptr<std_srvs::srv::Empty::Request>,
        std::shared_ptr<std_srvs::srv::Empty::Response>)
    {
        running_ = true;
        RCLCPP_INFO(this->get_logger(), "Patrol resumed");
    }

    bool running_;

    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;

    rclcpp::Service<std_srvs::srv::Empty>::SharedPtr stop_service_;
    rclcpp::Service<std_srvs::srv::Empty>::SharedPtr continue_service_;

    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    auto node = std::make_shared<PatrolController>();

    rclcpp::spin(node);

    rclcpp::shutdown();
    return 0;
}