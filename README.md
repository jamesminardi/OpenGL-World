# OpenGL-World


## Build with Source

### Requirements:
* [`cmake`](https://cmake.org) 3.10+
* a modern C++17 compiler (`gcc-8`, `clang-6.0`, `MSVC 2017` or above)

### Setup


##### 1. Cloning the repository
Project dependencies are git submodules under `external/`.

Use: 
```
git clone ...
git submodule update --init --recursive
```
or
```
git clone --recurse-submodules ...
```
#### 2. CMake

*Note:* The project currently requires the CMake build to be two directories deep from the top-level directory.

e.g.
Run CMake under the directory `OpenGL-World/build/cmake-build/`,

`cmake -G "MinGW Makefiles" ../../`

The project can now be built using the resulting makefile in the case of the above example.
