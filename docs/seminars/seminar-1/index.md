# ROS 2 alapok

Ezen a gyakorlaton a ROS 2 alapjaival ismerkedünk meg. A cél az, hogy a gyakorlat végére mindenki kényelmesen el tudjon navigálni egy munkakörnyezeten belül, megértse a ROS 2 alapvető node-alapú architektúráját, és képes legyen mind Publish/Subscribe (Topic), mind Client/Server (Service) kommunikációt megvalósítani.

!!! note "Megjegyzés"
    A feladatok megoldása elérhető az útmutatóban, illetve az `education` repóban is megtalálható a kész megoldás a [`humble` branchen](https://github.com/kroshu/education/tree/humble/VIIIAV55).

## 1. Munkakörnyezet (Workspace) felépítése

- Mi az a `colcon` és a workspace szerkezete (`src`, `build`, `install`, `log`)
- Környezeti változók beállítása (`source install/setup.bash`)

## 2. ROS 2 csomag (Package) létrehozása

Hozz létre egy új Python alapú ROS 2 csomagot. Ehhez használd a `ros2 pkg create` parancsot.

Vizsgáld is meg a csomag tartalmát.

??? example "Megoldás"
    ```console
    ros2 pkg create --build-type ament_python --node-name my_first_node --destination-directory src/VIIIAV55/ my_first_node
    ```

## 3. Az első Node megírása

Az újonnan létrehozott csomagban hozz létre egy osztályt, ami `Node` ősből származik le. Ennek az osztálynak legyen egy függvénye, amit meg tudunk hívni és kiír valamit a konzolra.

??? example "Megoldás"
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

## 4. Node-ok közötti kommunikáció I.: Topic

Nyisd meg a `publisher.cpp` fájlt. Ebbe a fájlba fogsz létrehozni egy topic-ot.

### Publisher node létrehozása

Hozz létre egy node-ot, aminek van két paramétere, `greeting` és `to_greet`. Ezeket a paramétereket egy üzenet formájában írja ki a node a korábban létrehozott topic-ra másodpercenként. Figyelj oda arra, hogy a paraméterek értékei futásidőben is megváltozhatnak!

??? example "Megoldás"
    ```cpp
    // MinimalPublisher::MinimalPublisher()
    this->declare_parameter("to_greet", "world");
    this->declare_parameter("greeting", "Hello");
    
    this->to_greet = this->get_parameter("to_greet").as_string();
    this->greeting = this->get_parameter("greeting").as_string();

    publisher_ = this->create_publisher<std_msgs::msg::String>("topic", 10);
    timer_ = this->create_wall_timer(1000ms, std::bind(&MinimalPublisher::timer_callback, this));

    // MinimalPublisher::timer_callback()
    this->get_parameter("to_greet", this->to_greet);
    this->get_parameter("greeting", this->greeting);

    auto message = std_msgs::msg::String();
    message.data = this->greeting + ", " + this->to_greet + "! " + std::to_string(count_++);
    RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str());
    publisher_->publish(message);
    ```

### Subscriber node létrehozása

Most, hogy létrehoztál egy publishert, készíts egy olyan node-ot is, ami feliratkozik a kipublikált üzenetekre a `subscriber.cpp` fájlban.

Készíts egy olyan osztályt, ami feliratkozik a korábban létrehozott topic-ra és kiírja a konzolra a kapott üzenetet. 

??? example "Megoldás"
    ```cpp
    // MinimalSubscriber::MinimalSubscriber()
    subscription_ = this->create_subscription<std_msgs::msg::String>(
    "topic",
    10,
    std::bind(
        &MinimalSubscriber::topic_callback,
        this,
        std::placeholders::_1));

    // MinimalSubscriber::topic_callback()
    RCLCPP_INFO(this->get_logger(), "I heard: '%s'", msg->data.c_str());
    ```

## 5. Node-ok közötti kommunikáció II.: Service

Eddig láthattuk azt, hogy miképpen válthat üzenetet két node, amelyek adatfolyam-alapon működnek. Most megismerkedünk a kérés-válasz alapú node-ok világával is. Ehhez egy olyan service-t hozunk létre, amely a megadott születési dátum alapján kiszámítja az életkort.

### Saját interfész definiálása

A ROS 2 világában rengeteg [üzenettípust definiáltak](https://docs.ros.org/en/humble/Concepts/Basic/About-Interfaces.html) már korábban. Ugyanakkor előfordulhat, hogy valami sajátos interfészre lenne szükségünk. Természetesen erre is van lehetőségünk. Ehhez használjuk a `GetAge.srv` fájlt.

Definiálj egy olyan interfészt, amely egy születési dátumot (`string`) fogad és egy életkorral (`int64`) válaszol.

??? example "Megoldás"
    ```txt
    string birth_date
    ---
    int64 age
    ```

### Service szerver létrehozása

Nyisd meg a `get_age_server.py` fájlt. Ebben definiáld azt a servicet, ami `GetAge` típusú interfészen keresztül kommunikál. Amikor ez a service egy kérést kap, válaszoljon a dátum alapján kiszámított életkorral.

??? example "Megoldás"
    ```python
    # __init__
    self.srv = self.create_service(GetAge, "get_age", self.get_age_callback)
    self.logger.info("GetAge server successfully initialized")

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

### Service kliens létrehozása

A `get_age_client.py` fájlon belül most hozz létre egy olyan node-ot, ami egy paraméteren keresztül beállított dátummal meghívja a szervert. Fontos, hogy a kliens megvárja, amíg a szerver elérhetővé válik.

??? example "Megoldás"
    ```python
    # __init__
    self.declare_parameter("birth_date", "1990.01.01.")
    self.birth_date = (
        self.get_parameter("birth_date").get_parameter_value().string_value
    )

    self.cli = self.create_client(GetAge, "get_age")

    while not self.cli.wait_for_service(timeout_sec=1.0):
        self.logger.info("service not available, waiting again...")

    self.req = GetAge.Request()
    self.logger.info("GetAge client succesfully initialized")

    # send_request
    self.logger.info(
        f"Sending request to get_age service with birth date: {self.birth_date}"
    )
    self.req.birth_date = self.birth_date
    return self.cli.call_async(self.req)

    # main
    client = GetAgeClient()
    future = client.send_request()

    rclpy.spin_until_future_complete(client, future)
    response = future.result()
    client.logger.info(f"Result of get_age: {response.age}")

    client.destroy_node()
    ```

## 6. Hasznos parancssori eszközök (CLI)

- `ros2 node list` / `ros2 node info`
- `ros2 topic list` / `ros2 topic echo` / `ros2 topic pub`
- `ros2 service list` / `ros2 service call` / `ros2 service type`
- `rqt`

## Ha többre vagy kíváncsi

- [Hivatalos ROS 2 útmutatók](https://docs.ros.org/en/humble/Tutorials.html)
- [Kevin Wood YouTube csatornája](https://www.youtube.com/@kevinwoodrobotics/videos)
