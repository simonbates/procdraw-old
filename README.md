ProcDraw
========

Dependencies:

- Visual Studio Express 2013 for Desktop
- DirectX SDK
- CMake

Run CMake to generate the Visual Studio solution:

```
mkdir build
cd build
cmake -G "Visual Studio 12" ..
```

To build in Visual Studio:

- Open build\ProcDraw.sln
- Build Solution (F7)

To run ProcDraw from Visual Studio:

- Set procdraw_winmain as the Startup Project
- Start Debugging (F5) or Start Without Debugging (Ctrl+F5)

To run the tests (from the build directory):

```
ctest --output-on-failure
```
