# Procdraw

Procdraw is an experimental live programming environment.

## Building

### Prerequisites

- Visual Studio 2019 C++ command line build tools, with components:
    - MSVC v142
    - Windows 10 SDK
    - C++ CMake tools for Windows
- Vcpkg

### Install dependencies with Vcpkg

    > vcpkg install glfw3 glm gtest plog

### Build

Run the following in a Visual Studio "Developer Command Prompt":

    > mkdir build
    > cd build
    > cmake -G Ninja -D CMAKE_TOOLCHAIN_FILE=[vcpkg root]\scripts\buildsystems\vcpkg.cmake ..
    > cmake --build .

## Dependencies

Procdraw uses these awesome open source projects and libraries:

| Dependency | License |
| :--------- | :------ |
| [github.com/Dav1dde/glad](https://github.com/Dav1dde/glad) | MIT License |
| [github.com/g-truc/glm](https://github.com/g-truc/glm) | MIT License |
| [github.com/glfw/glfw](https://github.com/glfw/glfw) | Zlib License |
| [github.com/google/googletest](https://github.com/google/googletest) | BSD 3-Clause "New" or "Revised" License |
| [github.com/SergiusTheBest/plog](https://github.com/SergiusTheBest/plog) | Mozilla Public License 2.0 |
| [github.com/yaml/pyyaml](https://github.com/yaml/pyyaml) | MIT License |
