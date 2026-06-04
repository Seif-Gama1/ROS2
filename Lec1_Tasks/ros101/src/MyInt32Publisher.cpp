#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int32.hpp"

//allows to write 500ms instead of std::chrono::milliseconds(500)
using namespace std::chrono_literals;

class MyInt32Publisher : public rclcpp::Node {
public:
  MyInt32Publisher() : Node("my_int32_publisher"), count_(0) {
    // Create the publisher with topic name "topic" and queue size 3
    publisher_ = this->create_publisher<std_msgs::msg::Int32>("my_int32_topic", 3);
    // Create a timer that executes the timer_callback every 500ms
    timer_ = this->create_wall_timer(
      750ms, std::bind(&MyInt32Publisher::timer_callback, this));
  }

private:
  void timer_callback() {
    auto message = std_msgs::msg::Int32();
    message.data = count_++;
    RCLCPP_INFO(this->get_logger(), "Publishing: %d", message.data); /* this is equivalent to printf (debugging in ROS) */
    publisher_->publish(message);
  }

  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr publisher_;
  size_t count_;
};

int main(int argc, char * argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MyInt32Publisher>());
  rclcpp::shutdown();
  return 0;
}
