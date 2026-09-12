# Robot Control with a Custom Controller

In this exercise, you need to implement a **ros2_control** controller that:

- follows joint-position targets arriving on a ROS topic,
- can accept updated targets while moving,
- is configurable at runtime (`max_velocity`, `kp`).

Starter files are available in `VIIIAV55/my_controller`.

## Running the code

Use the commands below to build and run your implementation.

```shell
cd ~/ros2_ws
colcon build --packages-select my_controller
source install/setup.bash
ros2 launch my_controller startup.launch.py
```

In another terminal:

```shell
source ~/ros2_ws/install/setup.bash
ros2 lifecycle set /robot_manager configure
ros2 lifecycle set /robot_manager activate
```

## Using the controller

Publish joint targets to the topic you configure in your subscription callback. Example:

```shell
source ~/ros2_ws/install/setup.bash
ros2 topic pub my_controller/command std_msgs/msg/Float64MultiArray "{data: [0.0, -1.57, 1.57, 0.0, 1.57, 0.0]}" --once
```

Useful commands for verification:

```shell
ros2 control list_controllers
ros2 control list_hardware_interfaces
ros2 topic echo /joint_states
```

## Expected result

- The robot approaches target positions smoothly.
- If a new target arrives, motion continues directly toward the new target.
- Motion speed stays within the configured `max_velocity` limit.

## Task details

The implementation logic is marked with `TODO` comments in `src/my_controller.cpp`.

### 1. Read parameters

In `on_configure()`, read the following parameters from `my_controller.yaml`:

- `max_velocity`
- `kp`

Then initialize internal variables:

- `num_joints_`
- `target_cache_`

??? example "Solution"
    ```C++
    controller_interface::CallbackReturn MyController::on_configure(const rclcpp_lifecycle::State &)
    {
      max_velocity_ = get_node()->get_parameter("max_velocity").as_double();
      kp_ = get_node()->get_parameter("kp").as_double_array();
      num_joints_ = kp_.size();
      target_cache_.assign(num_joints_, 0.0);

      /* ... */

      return controller_interface::CallbackReturn::SUCCESS;
    }
    ```

### 2. Create subscription

In `on_configure()`, subscribe to incoming joint targets (`std_msgs/msg/Float64MultiArray`). Copy incoming data into the realtime buffer (`rt_target_pos_`).

!!! warning "Important"
    Do not run long or blocking operations in the callback. Forward incoming target data to the realtime buffer as quickly as possible.

??? example "Solution"
    ```C++
    controller_interface::CallbackReturn MyController::on_configure(const rclcpp_lifecycle::State &)
    {
      /* ... */

      cmd_sub_ = get_node()->create_subscription<std_msgs::msg::Float64MultiArray>(
        "~/command", rclcpp::SystemDefaultsQoS(),
        [this](const std_msgs::msg::Float64MultiArray::SharedPtr msg)
        {
          rt_target_pos_.writeFromNonRT(msg->data);
        });
      return controller_interface::CallbackReturn::SUCCESS;
    }
    ```

### 3-4. Configure interfaces

Build the correct interface lists:

- `command_interface_configuration()`: controlled interfaces (`joint_i/position`),
- `state_interface_configuration()`: read interfaces (`joint_i/position`).

??? tip "Hint"
    The `get_interface_configuration()` helper already creates the 6 interface names.

??? example "Solution"
    ```C++
    controller_interface::InterfaceConfiguration MyController::command_interface_configuration() const
    {
      return get_interface_configuration();
    }

    controller_interface::InterfaceConfiguration MyController::state_interface_configuration() const
    {
      return get_interface_configuration();
    }
    ```

### 5. Activation logic

In `on_activate()`:

- read current joint states,
- write them into the realtime target buffer,
- set command interfaces to the same values.

This avoids startup jumps.

??? example "Solution"
    ```C++
    controller_interface::CallbackReturn MyController::on_activate(const rclcpp_lifecycle::State &)
    {
      std::vector<double> current(num_joints_, 0.0);
      for (size_t i = 0; i < num_joints_; i++)
      {
        current[i] = state_interfaces_[i].get_value();
        command_interfaces_[i].set_value(current[i]);
      }
      rt_target_pos_.initRT(current);

      return controller_interface::CallbackReturn::SUCCESS;
    }
    ```

### 6-7. Control loop

In `update()`:

1. read the latest target vector from the realtime buffer,
2. compute per-joint error (`target - current`),
3. compute a velocity-limited step (`kp * error`, then clamp to `[-max_velocity_, +max_velocity_]`),
4. convert the step into a position command using elapsed time (`period.seconds()`),
5. write to the command interface.

??? example "Solution"
    ```C++
    controller_interface::return_type MyController::update(const rclcpp::Time &, const rclcpp::Duration &period)
    {
      std::copy_n(rt_target_pos_.readFromRT()->data(), num_joints_, target_cache_.data());

      const double max_step = max_velocity_ * period.seconds();
      for (size_t i = 0; i < num_joints_; i++)
      {
        const double error = target_cache_[i] - state_interfaces_[i].get_value();
        const double u = std::clamp(kp_[i] * error, -max_step, +max_step);
        command_interfaces_[i].set_value(state_interfaces_[i].get_value() + u);
      }

      return controller_interface::return_type::OK;
    }
    ```
