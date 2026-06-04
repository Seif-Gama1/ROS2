#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float32.hpp"
#include <fstream>

//allows to write 500ms instead of std::chrono::milliseconds(500)
using namespace std::chrono_literals;

class CPUTempPublisher : public rclcpp::Node {
public:
  CPUTempPublisher() : Node("cpu_temp_publisher"), count_(0) {
    // Create the publisher with topic name "topic" and queue size 3
    publisher_ = this->create_publisher<std_msgs::msg::Float32>("cpu_temp", 3);
        
    temp_file_.open("/sys/class/thermal/thermal_zone0/temp");
    
    // Create a timer that executes the timer_callback every 500ms
    timer_ = this->create_wall_timer(
      1000ms, std::bind(&CPUTempPublisher::timer_callback, this));
  }

private:
  void timer_callback() {
    auto message = std_msgs::msg::Float32();

    if (!temp_file_.is_open()) {
      RCLCPP_ERROR(this->get_logger(), "Temperature file not open");
      return;
    }

    temp_file_.clear();      // reset EOF/fail flags
    temp_file_.seekg(0);     // rewind to beginning

    std::string temp_str;
    std::getline(temp_file_, temp_str);

    try {
        float temp_milli = std::stof(temp_str);
        message.data = temp_milli / 1000.0f;

        RCLCPP_INFO(this->get_logger(), "CPU Temp: %.2f °C", message.data);

        publisher_->publish(message);

    } catch (const std::exception &e) {
        RCLCPP_ERROR(this->get_logger(), "Parse error: %s", e.what());
    }
  }

  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr publisher_;
  std::ifstream temp_file_;
  size_t count_;
};

int main(int argc, char * argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<CPUTempPublisher>());
  rclcpp::shutdown();
  return 0;
}
