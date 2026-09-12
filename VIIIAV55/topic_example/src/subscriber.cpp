#include <memory>
#include <functional>

#include "topic_example/subscriber.h"

namespace subscriber_example
{
    MinimalSubscriber::MinimalSubscriber() : Node("minimal_subscriber")
    {
        subscription_ = this->create_subscription<std_msgs::msg::String>(
            "topic",
            10,
            std::bind(
                &MinimalSubscriber::topic_callback,
                this,
                std::placeholders::_1));
    }

    void MinimalSubscriber::topic_callback(const std_msgs::msg::String::SharedPtr msg)
    {
        RCLCPP_INFO(this->get_logger(), "I heard: '%s'", msg->data.c_str());
    }
} // namespace subscriber_example

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<subscriber_example::MinimalSubscriber>());
    rclcpp::shutdown();
    return 0;
}
