#!/usr/bin/env bash
set -euo pipefail

rosdep update
sudo apt-get update
rosdep install --from-paths . --ignore-src --rosdistro humble -y
