# Procdraw

[![Build status](https://ci.appveyor.com/api/projects/status/4wiskg8t3hflfsuo?svg=true)](https://ci.appveyor.com/project/simonbates/procdraw)

Procdraw is a work-in-progress live programming environment.

## Building

### Prerequisites

- Visual Studio 2019 C++ command line build tools
    - The build tools can be installed by themselves, or as part of the Visual Studio IDE:
        - Visual Studio Build Tools 2019 with the "C++ build tools" workload
        - Or, Visual Studio 2019 with the "Desktop development with C++" workload
    - Required components:
        - MSVC v142
        - Windows 10 SDK
        - C++ CMake tools for Windows (for Ninja)
- Python 3

### Build

To build Procdraw, run the following in a Visual Studio "Developer Command Prompt":

    > python configure.py
    > ninja

The automated tests can be run with:

    > .\build\procdraw_tests.exe
