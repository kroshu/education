# Fejlesztői környezet előkészítése

A fejlesztői környezetet egy **Dev Container** formájában biztosítjuk. Erről a technológiáról részletesebben a [Visual Studio Code dokumentációjában](https://code.visualstudio.com/docs/devcontainers/containers) olvashatsz.

## 1. Előkövetelmények telepítése

Mielőtt elindítanád a konténert, telepítsd az alábbi szoftvereket:

- [Visual Studio Code](https://code.visualstudio.com/) + [Dev Containers](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-containers) kiegészítő
- [Git](https://git-scm.com/)
- [Docker Desktop](https://www.docker.com/products/docker-desktop)
    - **Windows esetén:**
        1. Telepítsd a [WSL2](https://learn.microsoft.com/en-us/windows/wsl/about) segédprogramot egy adminisztrátori PowerShell ablakból a `wsl --install` paranccsal, majd indítsd újra a számítógépet.
        2. A Docker Desktop telepítése során válaszd a **WSL2 backend** opciót.
        3. A telepítés után nyisd meg a Docker **Settings**-et, majd a **Resources > WSL integration** menüpontban engedélyezd az **Enable integration with my default WSL distro** és az **Ubuntu** opciókat.

## 2. A projekt előkészítése és indítása

**1. Repository klónozása**

Nyiss meg egy terminált (pl. PowerShell) és futtasd a következő parancsot:

```shell
git clone https://github.com/kroshu/education.git
```

**2. Docker daemon indítása**

Nyisd meg a **Docker Desktop** alkalmazást (ez automatikusan elindítja a háttérben futó [Docker szolgáltatást](https://docs.docker.com/engine/daemon/start/)).

**3. Mappa megnyitása VS Code-ban**

Nyisd meg az `education` repón belül található `VIIIAV55` nevű mappát VS Code-ban.

**4. Környezetfüggő módosítás (csak Windows esetén):**

Nyisd meg a `.devcontainer/devcontainer.json` fájlt, és kommentezd ki a **31–44. sorokat** (a `mounts`, `containerEnv` és `remoteEnv` kulcsokat).

**5. Dev Container felépítése**

Nyomj `F1`-et a `Command Palette` megnyitásához, majd válaszd a `Dev Containers: Rebuild and Reopen in Container` lehetőséget, és várd meg, amíg a rendszer felépül.
