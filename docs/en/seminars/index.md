# Development Environment Setup

## Dev Container

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
