# Practice Overview

In this practice session, we focus on two main topics:

- developing a [ros2_control](https://github.com/ros-controls/ros2_control) controller for a 6-axis robot,
- trying out some of the core features of the [MoveIt 2](https://github.com/moveit/moveit2) framework.

Our additional goal is to make sure everyone has a working ROS 2 development environment.

!!! note "Note"
    All solutions are available in this guide, and also in the education repository on the [`humble` branch](https://github.com/kroshu/education/tree/humble/VIIIAV55).

## Development environment setup

The required environment is provided as a **Dev Container**.
You can read more about this technology on the [Visual Studio Code website](https://code.visualstudio.com/docs/devcontainers/containers).

Required software to run the container:

- [Visual Studio Code](https://code.visualstudio.com/) + [Dev Containers](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-containers) extension
- [Docker Desktop](https://www.docker.com/products/docker-desktop) &ndash; technically [Docker Engine](https://docs.docker.com/engine/install/) would also be enough, but Dev Container documentation also recommends Desktop

If these are installed, clone the education repository to your machine. For example:

```shell
git clone https://github.com/kroshu/education.git
```

After that, start the [Docker daemon](https://docs.docker.com/engine/daemon/start/), which can be done easily from the Docker Desktop UI. Open the `VIIIAV55` folder in the education repository in VS Code. Press `F1` to open the Command Palette, then select `Dev Containers: Rebuild and Reopen in Container`. Then wait for the environment to build.

### Differences on corporate network

If you want to set up the environment from the **KUKA network**, first obtain the KUKA root certificate and place it in `.devcontainer/certs`. In addition, uncomment the section after the `mounts` key in `.devcontainer/devcontainer.json`. After that, everything is the same as above.

!!! note "Note"
    The above flow was tested with [Podman Desktop](https://podman-desktop.io/), which is an open-source alternative to Docker Desktop. Podman-specific settings are documented in the [VS Code docs](https://code.visualstudio.com/remote/advancedcontainers/docker-options#_podman).
