FROM ros:humble-ros-base

ARG USERNAME=rosdev
ARG UID=1000
ARG GID=$UID

RUN apt-get update -q \
    && apt-get upgrade -q -y \
    && apt-get install -y --no-install-recommends \
    software-properties-common \
    python3-pip \
    xauth \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

# Create and switch to user
RUN groupadd -g $GID $USERNAME \
    && useradd -lm -u $UID -g $USERNAME -s /bin/bash $USERNAME \
    && echo "$USERNAME ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers
USER $USERNAME

# Private runtime dir for Qt/GUI apps (the host's XDG_RUNTIME_DIR is not mounted)
ENV XDG_RUNTIME_DIR=/tmp/runtime-$USERNAME
RUN mkdir -p -m 0700 $XDG_RUNTIME_DIR

# Create workspace so that user own this directory
RUN mkdir -p /home/$USERNAME/ros2_ws/src/VIIIAV55
WORKDIR /home/$USERNAME/ros2_ws

# Copy configuration files
RUN echo 'export DISPLAY="${DISPLAY:-:0}"' >> /home/$USERNAME/.bashrc \
    && echo 'source /opt/ros/'$ROS_DISTRO'/setup.bash' >> /home/$USERNAME/.bashrc \
    && echo 'source /home/'$USERNAME'/ros2_ws/install/setup.bash' >> /home/$USERNAME/.bashrc

# Clone repositories
RUN git clone https://github.com/kroshu/kuka-external-control-sdk.git \
        /home/${USERNAME}/ros2_ws/src/kuka-external-control-sdk \
    && git clone -b humble https://github.com/kroshu/kuka_drivers.git \
        /home/${USERNAME}/ros2_ws/src/kuka_drivers \
    && git clone -b humble https://github.com/kroshu/kuka_robot_descriptions.git \
        /home/${USERNAME}/ros2_ws/src/kuka_robot_descriptions

RUN cd /home/${USERNAME}/ros2_ws/src/kuka-external-control-sdk && git checkout 6279e3e918e090507629cfb88a953e690d0ed3a4 \
    && cd /home/${USERNAME}/ros2_ws/src/kuka_drivers && git checkout 20bfcfc777dd891039ebfaa1aad501dca4e3a3aa \
    && cd /home/${USERNAME}/ros2_ws/src/kuka_robot_descriptions && git checkout f26cd854a42353ceb79f477d9d60680dbed51a69
