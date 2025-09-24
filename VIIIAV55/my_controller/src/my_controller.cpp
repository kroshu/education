#include <algorithm>

#include "hardware_interface/types/hardware_interface_type_values.hpp"
#include "my_controller/my_controller.hpp"

namespace my_controller
{
    controller_interface::CallbackReturn MyController::on_init()
    {
        return controller_interface::CallbackReturn::SUCCESS;
    }

    controller_interface::InterfaceConfiguration MyController::get_interface_configuration() const
    {
        controller_interface::InterfaceConfiguration conf;
        conf.type = controller_interface::interface_configuration_type::INDIVIDUAL;
        conf.names.reserve(num_joints_);
        for (size_t i = 0; i < num_joints_; i++)
        {
            conf.names.push_back("joint_" + std::to_string(i + 1) + "/" + hardware_interface::HW_IF_POSITION);
        }
        return conf;
    }

    controller_interface::InterfaceConfiguration MyController::command_interface_configuration() const
    {
        return get_interface_configuration();
    }

    controller_interface::InterfaceConfiguration MyController::state_interface_configuration() const
    {
        return get_interface_configuration();
    }

    controller_interface::CallbackReturn MyController::on_configure(const rclcpp_lifecycle::State &)
    {
        max_velocity_ = get_node()->get_parameter("max_velocity").as_double();
        kp_ = get_node()->get_parameter("kp").as_double_array();
        num_joints_ = kp_.size();
        target_cache_.assign(num_joints_, 0.0);
        cmd_sub_ = get_node()->create_subscription<std_msgs::msg::Float64MultiArray>(
            "~/command", rclcpp::SystemDefaultsQoS(),
            [this](const std_msgs::msg::Float64MultiArray::SharedPtr msg)
            {
                rt_target_pos_.writeFromNonRT(msg->data);
            });
        return controller_interface::CallbackReturn::SUCCESS;
    }

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

    controller_interface::return_type MyController::update(const rclcpp::Time &, const rclcpp::Duration &period)
    {
        // Get latest target
        std::copy_n(rt_target_pos_.readFromRT()->data(), num_joints_, target_cache_.data());

        // P-control
        const double max_step = max_velocity_ * period.seconds();
        for (size_t i = 0; i < num_joints_; i++)
        {
            const double error = target_cache_[i] - state_interfaces_[i].get_value();
            const double u = std::clamp(kp_[i] * error, -max_step, +max_step);
            command_interfaces_[i].set_value(state_interfaces_[i].get_value() + u);
        }

        return controller_interface::return_type::OK;
    }
} // namespace my_controller

#include "pluginlib/class_list_macros.hpp"
PLUGINLIB_EXPORT_CLASS(my_controller::MyController, controller_interface::ControllerInterface)
