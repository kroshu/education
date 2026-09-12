# Robot Motion with MoveIt

In this exercise, you will learn **MoveIt 2** fundamentals through two scenarios:

1. Place an object in the virtual scene and plan around it.
2. Implement a simple pick-and-place workflow.

Starter code is in `VIIIAV55/moveit_example`.

## Running the code

```shell
cd ~/ros2_ws
colcon build --packages-select moveit_example
source install/setup.bash
ros2 launch moveit_example startup.launch.py
```

!!! note "Note"
    The program is stepped through RViz Visual Tools prompts ("Press 'Next'...").

## Expected result

- In the first task, the robot avoids collisions while moving around the obstacle.
- In the second task, the robot successfully transfers the cylinder to the other table.
- During transport, the cylinder must not flip upside down.

## 1. Obstacle avoidance

The first example is in `include/moveit_example/first_scenario.hpp`.

Workflow:

1. Place a box-shaped obstacle in the **planning scene**.
2. Assign it a clearly visible color.
3. Plan to a target position while avoiding the object.

The starter code already includes:

- obstacle creation (`CollisionObject`),
- scene insertion (`PlanningSceneInterface`),
- a target pose,
- movement execution (`MoveToPose`).

## 2. Pick and Place

For the second example, complete `include/moveit_example/second_scenario.hpp`.

### 2.1. Approach pick position

- Read the cylinder position (`GetObjectPosition("cylinder")`).
- Create a `pick_up_pose` above the cylinder.
- Set the end-effector orientation to point downward.
- Move the robot to that pose (`MoveToPose`).

??? example "Solution"
    ```C++
    /* ... */

    // TODO 1
    moveit_visual_tools.prompt("Press 'Next' to plan to the cylinder");
    // Find the position of the cylinder
    moveit::planning_interface::PlanningSceneInterface planning_scene_interface;
    const auto cylinder_pos = GetObjectPosition("cylinder");
    RCLCPP_INFO(logger, "Cylinder position: %f %f %f", cylinder_pos.x, cylinder_pos.y, cylinder_pos.z);

    // Move the end-effector above the cylinder
    const auto pick_up_pose = [&cylinder_pos]
    {
      geometry_msgs::msg::Pose msg;

      msg.orientation.x = 0.0;
      msg.orientation.y = std::sin(Constants::PI / 2);
      msg.orientation.z = 0.0;
      msg.orientation.w = std::cos(Constants::PI / 2);

      msg.position.x = cylinder_pos.x;
      msg.position.y = cylinder_pos.y;
      msg.position.z = cylinder_pos.z + 0.06;
      return msg;
    }();
    MoveToPose(pick_up_pose, move_group_interface, moveit_visual_tools, logger);

    /* ... */
    ```

### 2.2. Pick cylinder (attach)

Simulate grasping by attaching the cylinder to the robot end-effector.

??? example "Solution"
    ```C++
    /* ... */

    // TODO 2
    move_group_interface.attachObject("cylinder");
    moveit_visual_tools.prompt("Cylinder attached to the end-effector. Press 'Next' to continue");

    /* ... */
    ```

### 2.3. Set orientation constraint

- Create an orientation constraint.
- Set a tolerance for allowed rotation.
- Apply the constraint through `move_group_interface`.

!!! note "Goal"
    Keep the end-effector pointing downward during transport.

??? example "Solution"
    ```C++
    /* ... */

    // TODO 3
    const auto constraints = []
    {
      moveit_msgs::msg::OrientationConstraint ocm;
      ocm.link_name = "tool0";
      ocm.header.frame_id = "world";

      ocm.orientation.x = 0.0;
      ocm.orientation.y = std::sin(Constants::PI / 2);
      ocm.orientation.z = 0.0;
      ocm.orientation.w = std::cos(Constants::PI / 2);

      // Specify max deviation in radians
      ocm.absolute_x_axis_tolerance = 0.5;
      ocm.absolute_y_axis_tolerance = 0.5;
      ocm.absolute_z_axis_tolerance = 0.5;
      ocm.weight = 1.0;

      moveit_msgs::msg::Constraints constraints;
      constraints.orientation_constraints.push_back(ocm);
      return constraints;
    }();
    move_group_interface.setPathConstraints(constraints);

    /* ... */
    ```

### 2.4. Reach drop-off point

- Read the target table position (`GetObjectPosition("table1")`).
- Create a `drop_off_pose` above that table.
- Move to the target pose while keeping the orientation constraint.

??? example "Solution"
    ```C++
    /* ... */

    // TODO 4
    // Find position of the first table
    const auto drop_off_position = GetObjectPosition("table1");
    RCLCPP_INFO(logger, "Target table position: %f %f %f", drop_off_position.x, drop_off_position.y, drop_off_position.z);

    // Move the end-effector to the drop off point
    const auto drop_off_pose = [&drop_off_position]
    {
      geometry_msgs::msg::Pose msg;

      msg.orientation.x = 0.0;
      msg.orientation.y = std::sin(Constants::PI / 2);
      msg.orientation.z = 0.0;
      msg.orientation.w = std::cos(Constants::PI / 2);

      msg.position.x = drop_off_position.x;
      msg.position.y = drop_off_position.y;
      msg.position.z = drop_off_position.z + 0.2 + 0.11 + 1e-3;
      return msg;
    }();
    move_group_interface.setPlanningTime(15.0);
    MoveToPose(drop_off_pose, move_group_interface, moveit_visual_tools, logger);

    /* ... */
    ```

### 2.5. Place cylinder (detach)

Detach the cylinder so it is placed on the table.

??? example "Solution"
    ```C++
    /* ... */

    // TODO 5
    move_group_interface.detachObject("cylinder");
    moveit_visual_tools.prompt("Cylinder detached from the end-effector. Press 'Next' to continue");

    /* ... */
    ```
