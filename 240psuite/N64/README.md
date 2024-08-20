## Developer Documentation

You can use a devcontainer in VSCode to ease development.

### A quickstart video tutorial on how to set up your environment 
[![Devcontainer quickstart guide](http://img.youtube.com/vi/h05ufOsRgZU/0.jpg)](http://www.youtube.com/watch?v=h05ufOsRgZU "Devcontainer quickstart guide").

## Building
Use `make clean && make all` within the terminal window.


## Submodules
To update all submodules to the latest versions, use `git submodule update --remote` from a terminal window when in the root directory.

### Libdragon
This is the underlying SDK used for N64 builds.
This repo currently uses the `preview` branch as a submodule at a specific commit.

### Tiny3D
This is a library used, located in the `libs` directory.
This repo currently uses the `main` branch as a submodule at a specific commit.
