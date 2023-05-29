# OpenGL-World

## About

### Dependencies
- [`GLM`](https://github.com/g-truc/glm) (math)
- [`GLFW`](https://github.com/glfw/glfw) (window creation)
- [`GEW`](https://github.com/Perlmint/glew-cmake) (OpenGL function loading)
- [`STB`](https://github.com/nothings/stb) (image loading)

(All of these are included)

## Setup

### Build Requirements:
- [`cmake`](https://cmake.org) 3.10+
- a modern C++17 compiler (`gcc-8`, `clang-6.0`, `MSVC 2017` or above)

(Only tested on Windows)
### To Build
- Clone this project using git.
- From the root of this project update all the submodules with `git submodule update --init --recusive`
- Open the project in an editor that directly supports CMAKE files or follow the command-line instructions below.
  ```
  mkdir build/release
  cd build/release
  cmake ../../
  ```
Use `cmake -G ${GENERATOR} ../../` instead to use a specified platform (i.e. `"Unix Makefiles"`)

## Project Structure
- `build`: This holds the generated build files and where the final binary applications are put
- `data`: This is where application images are stored
- `external`: This is where external dependencies are put. (i.e. git submodules)
- `resources`: This contains project resources such as textures, sounds, music, etc.
- `src`: This holds the source code for these examples


## Features
- Textured cube
- Light source
- Phong lighting model
- Direct State Access (DSA) OpenGL

### Controls
- `ESC`: Close the program

Camera
- `W`/`S`: Move camera up and down
- `A`/`D`: Move camera left and right
- `E`/`Q`: Move camera forward and backward

Light Source
- `I`/`K`: Move light source up and down
- `J`/`L`: Move light source left and right
- `O`/`U`: Move light source forward and backward
- `R`/`F`: Increase and decrease ambient light strength
- `T`/`G`: Increase and decrease diffused light strength



