# Procdraw

[![Build status](https://ci.appveyor.com/api/projects/status/4wiskg8t3hflfsuo?svg=true)](https://ci.appveyor.com/project/simonbates/procdraw)

## Building

Prerequisites:

- Visual Studio 2017 with the "Desktop development with C++" workload

Procdraw can be built in Visual Studio or on the command line.

To build Procdraw in Visual Studio:

- Open `Procdraw.sln`
- Select the Configuration and Platform (such as "Debug" and "x86")
- Run "Build Solution" from the "Build" menu
- The tests can be run using the built-in Test Explorer (Test/Windows/Test Explorer)

To build Procdraw on the command line:

- Open the VS 2017 "Developer Command Prompt"
- `msbuild Procdraw.sln /p:Configuration=Debug /p:Platform=x86`
- `vstest.console Debug\ProcdrawTests.dll`
