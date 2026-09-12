#include <memory>
#include <functional>

#include "topic_example/subscriber.h"

namespace subscriber_example
{
    MinimalSubscriber::MinimalSubscriber() : Node("minimal_subscriber")
    {
        // TODO 1
        // Create a subscription for std_msgs::msg::String messages on the "topic" topic
        // ...
    }

    void MinimalSubscriber::topic_callback(const std_msgs::msg::String::SharedPtr msg)
    {
        // TODO 2
        // Log the received message
        // ...
    }
} // namespace subscriber_example

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<subscriber_example::MinimalSubscriber>());
    rclcpp::shutdown();
    return 0;
}
