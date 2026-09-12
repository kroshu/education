# ROS 2 alapok

Ezen a gyakorlaton a ROS 2 alapjaival ismerkedünk meg. A cél az, hogy a
gyakorlat végére mindenki kényelmesen el tudjon navigálni egy munkakörnyezeten
belül, megértse a ROS 2 alapvető node-alapú architektúráját, és képes legyen
mind Publish/Subscribe (Topic), mind Client/Server (Service) kommunikációt
megvalósítani.

!!! note "Megjegyzés"
    A feladatok megoldása elérhető az útmutatóban, illetve az `education`
    repóban is megtalálhatató a kész megoldás a
    [`humble` branchen](https://github.com/kroshu/education/tree/humble/VIIIAV55).

## 1. Munkakörnyezet (Workspace) felépítése

- Mi az a `colcon` és a workspace szerkezete (`src`, `build`, `install`, `log`)
- Környezeti változók beállítása (`source install/setup.bash`)

## 2. ROS 2 csomag (Package) létrehozása

- Csomag generálása (`ros2 pkg create`)
- A `package.xml` és a build beállítások (`CMakeLists.txt` vagy `setup.py`)
  szerepe

## 3. Az első Node megírása

- Node mint objektum (C++ / Python alapok)
- Logolás a ROS 2-ben (`RCLCPP_INFO` / `get_logger()`)

## 4. Node-ok közötti kommunikáció I.: Topic-ok

- Adatfolyam-alapú, aszinkron kommunikáció (Publisher / Subscriber)
- Standard üzenettípusok (pl. `std_msgs`, `geometry_msgs`)
- Publisher és Subscriber Node megírása és kipróbálása

## 5. Node-ok közötti kommunikáció II.: Service-ek

- Kérés-válasz (Client / Server) alapú architektúra
- Mikor használjunk Topic-ot és mikor Service-t?
- Service Server és Service Client Node megírása

## 6. Konfiguráció és indítás

- Paraméterek beállítása Node-on belül és parancssorból
- Launch fájl (`.launch.py`) készítése több node egyidejű indításához

## 7. Hasznos parancssori eszközök (CLI)

- `ros2 node list` / `ros2 node info`
- `ros2 topic list` / `ros2 topic echo` / `ros2 topic pub`
- `ros2 service list` / `ros2 service call` / `ros2 service type`

## Ha többre vagy kíváncsi

- [Hivatalos ROS 2 útmutatók](https://docs.ros.org/en/humble/Tutorials.html)
- [Kevin Wood YouTube csatornája](https://www.youtube.com/@kevinwoodrobotics/videos)
