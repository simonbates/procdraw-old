# Procdraw

[![Build status](https://ci.appveyor.com/api/projects/status/4wiskg8t3hflfsuo?svg=true)](https://ci.appveyor.com/project/simonbates/procdraw)

## Building

Procdraw uses CMake with the Visual Studio command line tools.

Prerequisites:

- Visual Studio 2017 with the "Desktop development with C++" workload

(CMake is included with Visual Studio.)

To build Procdraw, open the VS 2017 "Developer Command Prompt", change directory to the `procdraw` working directory, and run:

    > mkdir build
    > cd build
    > cmake -G "NMake Makefiles" ..
    > nmake
    > nmake test
