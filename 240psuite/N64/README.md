## Developer Documentation

You can use a devcontainer in supported IDE's (such as VSCode) or the commandline to ease development.

### A quickstart video tutorial on how you could set up your environment 
[![Devcontainer quickstart guide](http://img.youtube.com/vi/h05ufOsRgZU/0.jpg)](http://www.youtube.com/watch?v=h05ufOsRgZU "Devcontainer quickstart guide").

## Building
Use `make clean && make all` within the terminal window.

## Libraries

### Tiny3D
This is a library used for 3d models, (located in the root workspace directory, when using a devcontainer).
***Note*** this currently uses a fork until https://github.com/HailToDodongo/tiny3d/pull/11 is available.
This repo currently uses the `main` branch and uses the latest commit.

## Submodules
To update all submodules to the latest versions, use `git submodule update --remote` from a terminal window when in the root directory.

### Libdragon
This is the underlying SDK used for N64 builds.
This repo currently uses the `preview` branch as a submodule at a specific commit.
