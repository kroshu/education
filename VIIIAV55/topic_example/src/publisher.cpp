#include <chrono>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "topic_example/publisher.h"

using namespace std::chrono_literals;
namespace publisher_example
{
    MinimalPublisher::MinimalPublisher() : Node("minimal_publisher"), count_(0)
    {
        // TODO 1
        // Declare parameters for the greeting and the name to greet
        // ...

        // TODO 2
        // Get the initial values of the parameters
        // ...

        // TODO 3
        // Create a publisher for std_msgs::msg::String messages on the "topic" topic
        // ...

        // TODO 4
        // Create a timer that calls the timer_callback function every second
        // ...
    }

    void MinimalPublisher::timer_callback()
    {
        // TODO 5
        // Update parameters in case they were changed
        // ...

        // TODO 6
        // Create a std_msgs::msg::String message and populate it with the greeting and name
        // ...

        // TODO 7
        // Log the message and publish it
        // ...
    }
}

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<publisher_example::MinimalPublisher>());
    rclcpp::shutdown();
    return 0;
}
