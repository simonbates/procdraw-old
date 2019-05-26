# Procdraw

[![Build status](https://ci.appveyor.com/api/projects/status/4wiskg8t3hflfsuo?svg=true)](https://ci.appveyor.com/project/simonbates/procdraw)

## Building

### Prerequisites

The Procdraw build is done using the Visual Studio 2019 C++ command line build tools. To get the build tools, install one of the following:

- Visual Studio Build Tools 2019 with the "C++ build tools" workload
- Or, Visual Studio 2019 with the "Desktop development with C++" workload

### Build

To build Procdraw, open a Visual Studio "Developer Command Prompt" and run:

    > nmake

To run the automated tests:

    > nmake test
