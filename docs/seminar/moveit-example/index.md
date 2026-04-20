# Robot mozgatása MoveIt segítségével

Ebben a feladatban két példán keresztül ismerkedünk meg a **MoveIt 2** alapjaival:

1. Egy objektumot helyezünk el a virtuális térben, amit a robotnak ki kell kerülnie.
2. Megvalósítunk egy egyszerű pick & place alkalmazást.

A kiinduló kód a `VIIIAV55/moveit_example` mappában található.

## Futtatás

```shell
cd ~/ros2_ws
colcon build --packages-select moveit_example
source install/setup.bash
ros2 launch moveit_example startup.launch.py
```

!!! note "Megjegyzés"
    Az alkalmazás RViz Visual Tools promptokkal lépteti a programot ("Press 'Next'...").

## Elvárt eredmény

- Az első feladatban a robot ütközésmentesen kerülje meg az akadályt.
- A második feladatban a hengert sikeres áthelyezi a robot a másik asztalra.
- Szállítás közben a henger ne forduljon fejjel lefelé.

## 1. Akadály kikerülsée

Az első példa a `include/moveit_example/first_scenario.hpp` fájlban található.

A feladat menete:

1. Helyezz el egy téglatest alakú akadályt a **planning scene**-ben.
2. Adj neki jól látható színt.
3. Terveztesd meg az útvonalat egy cél pozíció felé úgy, hogy a robot kerülje ki az objektumot.

Az előkészített kód már tartalmazza:

- az akadály létrehozását (`CollisionObject`),
- a scene-be illesztést (`PlanningSceneInterface`),
- egy célpozíciót,
- a mozgás meghívását (`MoveToPose`).

## 2. Pick and Place

A második példához a `include/moveit_example/second_scenario.hpp` fájlt kell kiegészíteni.

### 2.1. Felvételi pont megközelítése

- Kérd le a henger pozícióját (`GetObjectPosition("cylinder")`).
- Készíts egy `pick_up_pose` célpontot a henger fölé.
- Állítsd be, hogy a robot vége lefelé nézzen.
- Mozgasd a robotot ebbe a pozícióba (`MoveToPose`).

??? example "Megoldás"
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

### 2.2. Henger felvétele (attach)

Szimuláld a megfogást azzal, hogy a hengert a robot végéhez _kapcsolod_.

??? example "Megoldás"
    ```C++
    /* ... */

    // TODO 2
    move_group_interface.attachObject("cylinder");
    moveit_visual_tools.prompt("Cylinder attached to the end-effector. Press 'Next' to continue");

    /* ... */
    ```

### 2.3. Orientációs kényszer beállítása

- Hozz létre egy orientációs kényszert.
- Állíts be egy megengedő hibahatárt az elfordulásra.
- Add hozzá a kényszert a tervezéshez az `move_group_interface` objektumon segítségével.

!!! note "Cél"
    A robot a szállítás közben végig lefelé néző orientációban tartsa a hengert.

??? example "Megoldás"
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

### 2.4. Lerakási pont elérése

- Kérd le a másik asztal pozícióját (`GetObjectPosition("table1")`).
- Készíts egy `drop_off_pose` célpontot az asztal fölé.
- Mozgasd a robotot a megdott pozícióba a korábban beállított orientációs kényszer mellett.

??? example "Megoldás"
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

### 2.5. Henger lerakása (detach)

Rakd le a hengert, hogy az asztalra kerüljön.

??? example "Megoldás"
    ```C++
    move_group_interface.detachObject("cylinder");
    moveit_visual_tools.prompt("Cylinder detached from the end-effector. Press 'Next' to continue");
    ```
