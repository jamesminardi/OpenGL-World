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
- `src`: This holds the source code for these examples

## Functionality

