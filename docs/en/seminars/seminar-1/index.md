# ROS 2 Basics

In this seminar we get familiar with the basics of ROS 2. The goal is that by the end of the seminar everyone can comfortably navigate within a workspace, understand the fundamental node-based architecture of ROS 2, and be able to implement both Publish/Subscribe (Topic) and Client/Server (Service) communication.

!!! note "Note"
    Solutions to the exercises are available in the guide, and the finished solution can also be found in the `education` repo on the [`humble` branch](https://github.com/kroshu/education/tree/humble/VIIIAV55).

## 1. Workspace structure

- What is `colcon` and the structure of a workspace (`src`, `build`, `install`, `log`)
- Setting environment variables (`source install/setup.bash`)

## 2. Creating a ROS 2 package

Create a new Python-based ROS 2 package. Use the `ros2 pkg create` command for this.

Also examine the contents of the package.

??? example "Solution"
    ```console
    ros2 pkg create --build-type ament_python --node-name my_first_node --destination-directory src/VIIIAV55/ my_first_node
    ```

## 3. Writing your first Node

In the newly created package, create a class that derives from the `Node` base class. This class should have a function that we can call and that prints something to the console.

??? example "Solution"
    ```python
    from rclpy.node import Node
    import rclpy


    class MyFirstNode(Node):
        def __init__(self):
            super().__init__("my_first_node_name")

        def func_call(self):
            self.get_logger().info("Something, something...")


    def main():
        rclpy.init()
        n = MyFirstNode()
        n.func_call()
        rclpy.shutdown()


    if __name__ == "__main__":
        main()
    ```

To test the new node, first build the package in a terminal. In the current setup, it is recommended to build only this package and use the `--symlink-install` option to avoid unnecessary rebuilds later.

??? example "Solution"
    ```console
    colcon build --packages-select my_first_pkg --symlink-install
    ```

To run the node, it is recommended to open a new terminal and source the appropriate environment. After that, the node can be started.

!!! note "Note"
    Thanks to the `--symlink-install` option, changes made to the code in a Python package automatically take effect the next time the node is run after saving, so there is no need to rebuild after every modification.

??? example "Solution"
    ```console
    source install/setup.bash
    ros2 run my_first_pkg my_first_node
    ```


## 4. Communication between Nodes I.: Topic

Open the `publisher.cpp` file. In this file you will create a topic.

### Creating the Publisher node

Create a node that has two parameters, `greeting` and `to_greet`. The node writes these parameters in the form of a message to the previously created topic once every second. Pay attention to the fact that the values of the parameters can also change at runtime!

??? example "Solution"
    ```cpp
    // MinimalPublisher::MinimalPublisher()
    this->declare_parameter("to_greet", "world");
    this->declare_parameter("greeting", "Hello");
    
    this->to_greet = this->get_parameter("to_greet").as_string();
    this->greeting = this->get_parameter("greeting").as_string();
    ```
!!! note "Note"
The type used when reading a parameter must be specified.

Create a publisher named `topic` with message type `string`, and define a timer with a 1-second period that schedules the callback function responsible for publishing.

??? example "Solution"
    ```cpp
    // MinimalPublisher::MinimalPublisher()
    publisher_ = this->create_publisher<std_msgs::msg::String>("topic", 10);
    timer_ = this->create_wall_timer(
        1000ms,
        std::bind(&MinimalPublisher::timer_callback, this));
    ```

Publish the parameters read from the node as a message on the previously created topic. Pay attention to the fact that parameter values can change at runtime.

??? example "Solution"
    ```cpp
    // MinimalPublisher::timer_callback()
    this->get_parameter("to_greet", this->to_greet);
    this->get_parameter("greeting", this->greeting);

    auto message = std_msgs::msg::String();
    message.data = this->greeting + ", " + this->to_greet + "! " + std::to_string(count_++);
    RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str());
    publisher_->publish(message);
    ```

### Creating the Subscriber node

Now that you have created a publisher, also create a node that subscribes to the published messages in the `subscriber.cpp` file.

Create a class that subscribes to the previously created topic and prints the received message to the console.

??? example "Solution"
    ```cpp
    // MinimalSubscriber::MinimalSubscriber()
    subscription_ = this->create_subscription<std_msgs::msg::String>(
    "topic",
    10,
    std::bind(
        &MinimalSubscriber::topic_callback,
        this,
        std::placeholders::_1));
    ```

??? example "Solution"
    ```cpp
    // MinimalSubscriber::topic_callback()
    RCLCPP_INFO(this->get_logger(), "I heard: '%s'", msg->data.c_str());
    ```

## 5. Communication between Nodes II.: Service

So far we have seen how two nodes can exchange messages in a stream-based manner. Now we will also get familiar with the world of request-response based nodes. For this, we will create a service that calculates the age based on a given birth date.

### Defining your own interface

In the world of ROS 2, a lot of [message types have already been defined](https://docs.ros.org/en/humble/Concepts/Basic/About-Interfaces.html). However, it may happen that we need some specific interface. Naturally, we have the option to do this as well. For this, we use the `GetAge.srv` file.

Define an interface that accepts a birth date (`string`) and responds with an age (`int64`).

??? example "Solution"
    ```txt
    string birth_date
    ---
    int64 age
    ```

!!! note "Note"
    Custom message types (`.msg`, `.srv`, `.action`) can only be created in a C++ package. In larger projects it is recommended to organize custom interfaces into a dedicated package.

### Creating the Service server

Open the get_age_server.py file. In this file, define a service named get_age that communicates through a GetAge interface.

??? example "Solution"
    ```python
    # __init__
    self.srv = self.create_service(GetAge, "get_age", self.get_age_callback)
    self.logger.info("GetAge server successfully initialized")
    ```

When the service receives a request, it should respond with the age calculated from the provided birth date.

??? example "Solution"
    ```python
    # get_age_callback
    birth_string = request.birth_date.lower().strip()
    birth = datetime.strptime(birth_string, "%Y.%m.%d.").date()

    today = date.today()
    response.age = (
        today.year
        - birth.year
        - ((today.month, today.day) < (birth.month, birth.day))
    )
    self.logger.info(
        f"For birth date {birth_string} sending back age: {response.age}"
    )
    ```

### Creating the Service client

In the `get_age_client.py` file, create a node that calls the server with a date set through a parameter. It is important that the client waits until the server becomes available.

??? example "Solution"
    ```python
    # __init__
    self.declare_parameter("birth_date", "1990.01.01.")
    self.birth_date = (
        self.get_parameter("birth_date").get_parameter_value().string_value
    )
    ```

??? example "Solution"
    ```python
    self.cli = self.create_client(GetAge, "get_age")

    while not self.cli.wait_for_service(timeout_sec=1.0):
        self.logger.info("service not available, waiting again...")

    self.req = GetAge.Request()
    self.logger.info("GetAge client succesfully initialized")
    ```

??? example "Solution"
    ```python
    # send_request
    self.logger.info(
        f"Sending request to get_age service with birth date: {self.birth_date}"
    )
    self.req.birth_date = self.birth_date
    return self.cli.call_async(self.req)
    ```

??? example "Solution"
    ```python
    # main
    client = GetAgeClient()
    future = client.send_request()

    rclpy.spin_until_future_complete(client, future)
    response = future.result()
    client.logger.info(f"Result of get_age: {response.age}")

    client.destroy_node()
    ```

## 6. Useful command-line tools (CLI)

- `ros2 node list` / `ros2 node info`
- `ros2 topic list` / `ros2 topic echo` / `ros2 topic pub`
- `ros2 service list` / `ros2 service call` / `ros2 service type`
- `rqt`

## If you want to know more

- [Official ROS 2 tutorials](https://docs.ros.org/en/humble/Tutorials.html)
- [Kevin Wood's YouTube channel](https://www.youtube.com/@kevinwoodrobotics/videos)
