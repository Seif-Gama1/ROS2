#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float32.hpp"

using std::placeholders::_1;

class CPUTempSubscriber : public rclcpp::Node {
public:
  CPUTempSubscriber() : Node("cpu_temp_subscriber") {
    // Create the subscription listening on "topic" with queue size 10
    // _1 stands for "the first argument passed to the callback."
    // When ROS 2 receives a message, it calls your callback and "hands over" that message. 
    //_1 tells std::bind: "When you get an argument from ROS, take that first argument
    //  and plug it into the topic_callback function."

    subscription_ = this->create_subscription<std_msgs::msg::Float32>(
      "cpu_temp", 3, std::bind(&CPUTempSubscriber::topic_callback, this, _1));
      /* my comment: name of topic is "topic", queue size is 3, callback function is topic_callback */
  }

private:
  void topic_callback(const std_msgs::msg::Float32 & msg) const {
    RCLCPP_INFO(this->get_logger(), "I heard: %f", msg.data);
  }
  
  rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr subscription_;
};

int main(int argc, char * argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<CPUTempSubscriber>());
  rclcpp::shutdown();
  return 0;
}
