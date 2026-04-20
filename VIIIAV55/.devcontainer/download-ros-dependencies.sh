#!/usr/bin/env bash
set -euo pipefail

rosdep update
sudo apt-get update
sudo apt-get upgrade -y
rosdep install --from-paths . --ignore-src --rosdistro humble -y

# Install further ROS-related tools
sudo apt-get install ros-humble-rqt* -y
