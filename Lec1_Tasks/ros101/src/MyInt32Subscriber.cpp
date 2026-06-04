#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int32.hpp"

using std::placeholders::_1;

class MyInt32Subscriber : public rclcpp::Node {
public:
  MyInt32Subscriber() : Node("my_int32_subscriber") {
    // Create the subscription listening on "topic" with queue size 10
    // _1 stands for "the first argument passed to the callback."
    // When ROS 2 receives a message, it calls your callback and "hands over" that message. 
    //_1 tells std::bind: "When you get an argument from ROS, take that first argument
    //  and plug it into the topic_callback function."

    subscription_ = this->create_subscription<std_msgs::msg::Int32>(
      "my_int32_topic", 3, std::bind(&MyInt32Subscriber::topic_callback, this, _1));
      /* my comment: name of topic is "topic", queue size is 3, callback function is topic_callback */
  }

private:
  void topic_callback(const std_msgs::msg::Int32 & msg) const {
    RCLCPP_INFO(this->get_logger(), "I heard: %d", msg.data);
  }
  
  rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr subscription_;
};

int main(int argc, char * argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MyInt32Subscriber>());
  rclcpp::shutdown();
  return 0;
}
