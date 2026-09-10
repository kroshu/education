#include <chrono>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "topic_example/publisher.hpp"

using namespace std::chrono_literals;
namespace publisher_example
{
    MinimalPublisher::MinimalPublisher(): Node("minimal_publisher"), count_(0)
    {
        this->declare_parameter("to_greet", "world");
        this->to_greet = this->get_parameter("to_greet").as_string();
        this->declare_parameter("greeting", "Hello");
        this->greeting = this->get_parameter("greeting").as_string();
        publisher_ = this->create_publisher<std_msgs::msg::String>("topic", 10);
        timer_ = this->create_wall_timer(1000ms, std::bind(&MinimalPublisher::timer_callback, this));
    }

    void MinimalPublisher::timer_callback()
    {
        auto message = std_msgs::msg::String();
        message.data = this->greeting + ", " + this->to_greet + "! " + std::to_string(count_++);
        RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str());
        publisher_->publish(message);
    }
}

int main(int argc, char * argv[])
{
rclcpp::init(argc, argv);
rclcpp::spin(std::make_shared<publisher_example::MinimalPublisher>());
rclcpp::shutdown();
return 0;
}