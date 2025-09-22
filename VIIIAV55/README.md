# Robot operációs rendszerek és fejlesztői ökoszisztémák

## Gyakorlat menete

A gyakorlat során két fő témakörrel foglalkozunk:

1. [ros2_control](https://github.com/ros-controls/ros2_control) kontroller fejlesztése hattengelyes robothoz
2. [MoveIt 2](https://github.com/moveit/moveit2) keretrendszer alapvető funkcióinak megismerése

Célunk továbbá, hogy mindenkinél legyen egy működőképes ROS 2 fejlesztői környezet.

## 1. Robot vezérlése saját kontrollerrel

Ebben a feladatban egy olyan kontrollert kell készíteni, amely egy ROS topicon keresztül kapott csuklópozíciókat követve mozgatja a robotot. A kontrollernek képesnek kell lennie új célpozíció fogadására akkor is, ha az előző célt még nem érte el. További követelmény, hogy a kontroller legyen paraméterezhető egy konfigurációs fájlon keresztül. A kiinduló ROS csomagot a `my_controller` mappában találjátok.

## 2. Robot mozgatása MoveIt segítségével

A második részben két példán keresztül ismerkedünk meg a MoveIt alapjaival:

1. Egy objektumot helyezünk el a virtuális térben, amit a robotnak ki kell kerülnie.
2. Megvalósítunk egy egyszerű pick & place alkalmazást.

A kiinduláshoz szükséges kódot a `moveit_example` mappában találjátok. Először a `first_scenario.hpp` fájlt egészítjük ki, ahol egy téglatestet helyezünk el a világban, majd a robotot a túloldalára mozgatjuk. Ezután a `second_scenario.hpp` fájlban egy előre elkészített környezetben kell a lila hengert egyik asztalról a másikra áthelyezni úgy, hogy az soha ne forduljon fejjel lefelé.

## Linkek

* [Tantárgyi adatlap](https://portal.vik.bme.hu/kepzes/targyak/VIIIAV55/)
* [ROS 2](https://github.com/ros2)
* [ros2_control](https://github.com/ros-controls/ros2_control)
* [MoveIt 2](https://github.com/moveit/moveit2)
* [Sajtómegjelenés](https://www.linkedin.com/posts/kukaglobal_a-kuka-%C3%A9s-a-bme-budapest-university-of-activity-7229396384574013440-LHqc/)
