# Gyakorlat menete

A gyakorlat során két fő témakörrel foglalkozunk:

- [ros2_control](https://github.com/ros-controls/ros2_control) kontroller fejlesztése hattengelyes robothoz,
- [MoveIt 2](https://github.com/moveit/moveit2) keretrendszer alapvető funkcióinak megismerése.

Célunk továbbá, hogy mindenkinél legyen egy működőképes ROS 2 fejlesztői környezet.

!!! note "Megjegyzés"
    A feladatok megoldása elérhető az útmutatóban, illetve az `education` repóban is megtalálhatató a kész megoldás a [`humble`](https://github.com/kroshu/education/tree/humble/VIIIAV55) branchen.

## Fejlesztői környezet előkészítése

A szükséges környezetet egy **Dev Container** formájában biztosítottuk.
Erről a technológiáról részletesebben a [Visual Studio Code weboldalán](https://code.visualstudio.com/docs/devcontainers/containers) olvashattok.

A konténer indításához szükséges szoftverek:

- [Visual Studio Code](https://code.visualstudio.com/) + [Dev Containers](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-containers) kiegészítő
- [Docker Desktop](https://www.docker.com/products/docker-desktop) &ndash; valójában a [Docker Engine](https://docs.docker.com/engine/install/) is elegendő lenne, de a Dev Containerekről szóló dokumentáció is a Desktop verziót ajánlja

Ha ezek mind telepítve vannak, akkor leklónozhatjuk az `education` repository-t a saját gépünkre. Parancssorból például:

```shell
git clone https://github.com/kroshu/education.git
```

Ezután indítsuk el a [Docker daemont](https://docs.docker.com/engine/daemon/start/), ezt könnyen megtehetjük a Docker Desktop grafikus felületén keresztül is. Nyissuk meg az `education` repóban található `VIIIAV55` nevű mappát VS Code-ban. Nyomjuk meg az `F1` gombot a `Command Palette` megnyitásához, majd válasszuk ki a `Dev Containers: Rebuild and Reopen in Container` opciót. Most már csak várnunk kell, hogy felépüljön a rendszer.

### Eltérések céges hálózat esetén

Amennyiben a **KUKA hálózatáról** szeretnénk a környezetet összeállítani, akkor először meg kell szereznünk a KUKA gyökértanusítványát (*root certificate*), amit a `.devcontainer/certs` mappába kell elhelyeznünk. Ezen kívül ki kell kommentezni a `.devcontainer/devcontainer.json` fájlban a `mounts` kulcs utáni részt. Ezt követően minden ugyanúgy történik, mint a korábban leírt esetben.

!!! note "Megjegyzés"
    A fentebb leírt lépéseket [Podman Desktop](https://podman-desktop.io/) mellett próbáltuk ki, amely a Docker Desktop egy nyílt forráskódú alternatívája. Podman használatához szükséges beállításokról a [VS Code dokumentációjában](https://code.visualstudio.com/remote/advancedcontainers/docker-options#_podman) olvashattok.
