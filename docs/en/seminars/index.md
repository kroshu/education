# Development Environment Setup

The development environment is provided as a **Dev Container**. You can read more about this technology in the [Visual Studio Code documentation](https://code.visualstudio.com/docs/devcontainers/containers).

## 1. Install prerequisites

Before starting the container, install the following software:

- [Visual Studio Code](https://code.visualstudio.com/) + [Dev Containers](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-containers) extension
- [Git](https://git-scm.com/)
- [Docker Desktop](https://www.docker.com/products/docker-desktop)
    - **On Windows:**
        1. Install [WSL2](https://learn.microsoft.com/en-us/windows/wsl/about) from an administrator PowerShell window with the `wsl --install` command, then restart your computer.
        2. During the Docker Desktop installation, choose the **WSL2 backend** option.
        3. After installation, open Docker **Settings**, then under **Resources > WSL integration** enable the **Enable integration with my default WSL distro** and **Ubuntu** options.

## 2. Preparing and starting the project

**1. Clone the repository**

Open a terminal (e.g. PowerShell) and run the following command:

```shell
git clone https://github.com/kroshu/education.git
```

**2. Start the Docker daemon**

Open the **Docker Desktop** application (this automatically starts the [Docker service](https://docs.docker.com/engine/daemon/start/) in the background).

**3. Open the folder in VS Code**

Open the `VIIIAV55` folder inside the `education` repository in VS Code.

**4. Environment-specific modification (Windows only):**

Open the `.devcontainer/devcontainer.json` file and comment out **lines 31–44** (the `mounts`, `containerEnv` and `remoteEnv` keys).

**5. Enable X11 (Linux / Ubuntu only)**

Since we also want to run graphical applications (e.g. RViz) from the container, you need to allow local X11 connections on your host machine. Run the following command in a terminal:

```shell
xhost +local:root
```

**6. Build the Dev Container**

Press `F1` to open the `Command Palette`, then select `Dev Containers: Rebuild and Reopen in Container` and wait for the environment to build.
