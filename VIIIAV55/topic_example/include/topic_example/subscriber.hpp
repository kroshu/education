#ifndef TOPIC_EXAMPLE__MINIMAL_SUBSCRIBER_HPP_
#define TOPIC_EXAMPLE__MINIMAL_SUBSCRIBER_HPP_

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

namespace subscriber_example
{

class MinimalSubscriber : public rclcpp::Node
{
public:
    MinimalSubscriber();

private:
    void topic_callback(const std_msgs::msg::String::SharedPtr msg);

    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
};

}  // namespace subscriber_example

#endif  // TOPIC_EXAMPLE__MINIMAL_SUBSCRIBER_HPP_