# Procdraw

[![Build status](https://ci.appveyor.com/api/projects/status/4wiskg8t3hflfsuo?svg=true)](https://ci.appveyor.com/project/simonbates/procdraw)

## Building

### Prerequisites

The Procdraw build is managed by an NMAKE Makefile and uses the Visual C++ command line build tools. NMAKE and the other build tools are distributed as part of Visual Studio. To get the build tools, install:

- Visual Studio 2017 with the "Desktop development with C++" workload

### Build

To build Procdraw, open a Visual Studio "Developer Command Prompt" and run:

    > nmake

To run the automated tests:

    > nmake test
