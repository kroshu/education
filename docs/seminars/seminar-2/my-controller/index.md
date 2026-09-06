# Robot vezérlése saját kontrollerrel

Ebben a feladatban egy olyan **ros2_control** kontrollert kell elkészíteni, amely:

- ROS topicon érkező csuklópozíció-célokat követ,
- menet közben is képes új célértékeket fogadni,
- újrafordítás nélkül paraméterezhető (`max_velocity`, `kp`).

A kiinduló fájlok a `VIIIAV55/my_controller` mappában találhatók.

## Futtatás

Az elkészített kód futtatásához az alábbi parancsokat kell kiadni.

```shell
cd ~/ros2_ws
colcon build --packages-select my_controller
source install/setup.bash
ros2 launch my_controller startup.launch.py
```

Egy másik terminálban:

```shell
source ~/ros2_ws/install/setup.bash
ros2 lifecycle set /robot_manager configure
ros2 lifecycle set /robot_manager activate
```

## Kontroller használata

Publikálj csuklócélokat arra a topicra, amelyet a feliratkozásnál konfigurálsz. Példa:

```shell
source ~/ros2_ws/install/setup.bash
ros2 topic pub my_controller/command std_msgs/msg/Float64MultiArray "{data: [0.0, -1.57, 1.57, 0.0, 1.57, 0.0]}" --once
```

Ellenőrzéshez hasznos parancsok:

```shell
ros2 control list_controllers
ros2 control list_hardware_interfaces
ros2 topic echo /joint_states
```

## Elvárt eredmény

- A robot simán közelít a célpozíciókhoz.
- Új cél beérkezésekor nem áll meg, hanem azonnal az új célra vált.
- A mozgás sebessége nem lépi túl a `max_velocity` korlátot.

## Feladatleírás

A megvalósítandó logika a `src/my_controller.cpp` fájlban található `TODO` pontokban van jelölve.

### 1. Paraméterek beolvasása

Az `on_configure()` függvényben olvasd be a `my_controller.yaml` fájlból a következő paramétereket:

- `max_velocity`
- `kp`

Majd inicializáld a belső változókat:

- `num_joints_`
- `target_cache_`

??? example "Megoldás"
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

### 2. Feliratkozás létrehozása

Az `on_configure()` függvényben iratkozz fel a beérkező célcsuklószögekre (`std_msgs/msg/Float64MultiArray`). Az érkező üzenet tartalmát másold a valós idejű pufferbe (`rt_target_pos_`).

!!! warning "Fontos"
    A callbackben ne végezz hosszadalmas, blokkoló műveleteket. A beérkező célértéket gyorsan továbbítsd a valósidejű puffer felé.

??? example "Megoldás"
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

### 3&ndash;4. Interfészek konfigurálása

Állítsd össze a megfelelő interfészlistát:

- `command_interface_configuration()`: a vezérelt interfészek (`joint_i/position`),
- `state_interface_configuration()`: az olvasott interfészek (`joint_i/position`).

??? tip "Segítség"
    A `get_interface_configuration()` segédmetódus már előállítja a 6 darab interfésznevet.

??? example "Megoldás"
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

### 5. Aktivációs logika

Az `on_activate()` során:

- olvasd ki az aktuális csuklóállapotokat,
- írd ezeket a valósidejű célpufferbe,
- állítsd a command interfészeket is ezekre az értékekre.

Ezzel elkerülhető az induláskori ugrás.

??? example "Megoldás"
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

### 6&ndash;7. Szabályzási ciklus

Az `update()` függvényben:

1. olvasd ki a legfrissebb célvektort a valósidejű pufferből,
2. számolj csuklónkénti hibát (`target - current`),
3. számítsd ki a sebességkorlátos lépést (`kp * hiba`, majd szorítsd azt a konfigurált határok közé `[-max_velocity_, +max_velocity_]`),
4. a lépésből számíts új pozícióparancsot az eltelt idővel (`period.seconds()`),
5. írd be a command interfészbe.

??? example "Megoldás"
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
