# based on nanoarch: https://github.com/heuripedes/nanoarch

nanoarch is a small libretro frontend (nanoarch.c has less than 1000 lines of
code) created for educational purposes. It only provides the required (video,
audio and basic input) features to run most non-libretro-gl cores and there's
no UI or configuration support.

## Install dependencies
```bash
pacman -S libretro-gambatte glfw-x11 glu glew
```

## Building

Other than `make`, `cmake`, `pkg-config` and a working C99 or C++ compiler, you'll need
`alsa` and `glfw` development files installed.

Alternatively run g++ (or clang or c++ for standard system compiler) as follows:
```bash
g++ -Wall -pedantic -Wextra -ldl -lGL -lglfw -lGLEW -lasound nanoarch.cpp libretro.h -o nanoarch
```
## Running

```bash
chmod +x test.sh
./test.sh
```
