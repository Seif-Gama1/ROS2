#include "rclcpp/rclcpp.hpp"
#include "circular_moving_turtle/msg/robot_status.hpp"
#include "turtlesim/msg/pose.hpp"
#include "geometry_msgs/msg/pose2_d.hpp"
#include <cmath>
#include <chrono>

class StatusPublisher : public rclcpp::Node
{
public:
    StatusPublisher()
        : Node("status_publisher")
    {
        previous_theta_ = 0.0;
        accumulated_rotation_ = 0.0;
        lap_count_ = 0;

        this->declare_parameter("status_rate", 5.0);

        status_pub_ =
            this->create_publisher<
                circular_moving_turtle::msg::RobotStatus>(
                    "/robot/status", 10);

        pose_sub_ =
            this->create_subscription<
                turtlesim::msg::Pose>(
                    "/turtle1/pose", 10,
                    std::bind(
                        &StatusPublisher::pose_callback,
                        this,
                        std::placeholders::_1));

        status_rate_ = this->get_parameter("status_rate").as_double();
        auto period = std::chrono::duration<double>(1.0 / status_rate_);
        
        timer_ =
            this->create_wall_timer(
                std::chrono::duration_cast<std::chrono::milliseconds>(period),
                std::bind(
                    &StatusPublisher::publishStatus,
                    this));
    }

private:

    double status_rate_;
    rclcpp::Publisher<circular_moving_turtle::msg::RobotStatus>::SharedPtr status_pub_;
    rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr pose_sub_;
    turtlesim::msg::Pose current_pose_;
    rclcpp::TimerBase::SharedPtr timer_;
    float previous_theta_;
    float accumulated_rotation_;
    int lap_count_;
    bool first_pose_received_ = false;

    void publishStatus(){
        circular_moving_turtle::msg::RobotStatus status;

        status.pose.x = current_pose_.x;
        status.pose.y = current_pose_.y;
        status.pose.theta = current_pose_.theta;
        if (std::abs(current_pose_.linear_velocity) < 1e-6 &&
            std::abs(current_pose_.angular_velocity) < 1e-6)
        {
            status.state = "stopped";
        }else{
            status.state = "running";
        }
        status.temperature = 25.0 + std::abs(current_pose_.linear_velocity);
        status.lap_count = lap_count_;
        status_pub_->publish(status);
    }

    void pose_callback(const turtlesim::msg::Pose::SharedPtr msg)
    {
        if (!first_pose_received_) {
            previous_theta_ = msg->theta;
            first_pose_received_ = true;
            current_pose_ = *msg;
            return;
        }
        
        current_pose_ = *msg;

        float delta = current_pose_.theta - previous_theta_;
        
        if(delta > M_PI)
            delta -= 2*M_PI;

        if(delta < -M_PI)
            delta += 2*M_PI;
        
        accumulated_rotation_ += std::abs(delta);
        if(accumulated_rotation_ >= 2*M_PI){
            lap_count_++;
            accumulated_rotation_ -= 2*M_PI;
        }

        previous_theta_ = current_pose_.theta;
    }
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);

    auto node = std::make_shared<StatusPublisher>();

    rclcpp::spin(node);

    rclcpp::shutdown();
    return 0;
}