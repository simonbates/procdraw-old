ProcDraw
========

Building on Windows
-------------------

Dependencies:

- Visual Studio Community 2013
- CMake

Run CMake to generate the Visual Studio solution:

```
mkdir build
cd build
cmake -G "Visual Studio 12" ..
```

To build in Visual Studio:

- Open build\ProcDraw.sln
- Build Solution (Ctrl+Shift+B)

To run ProcDraw from Visual Studio:

- Set procdraw_winmain as the Startup Project
- Set the procdraw_winmain project Working Directory to the procdraw directory (Properties/Debugging/Working Directory)
- Start Debugging (F5) or Start Without Debugging (Ctrl+F5)

To run the tests (in the build directory):

```
ctest -C Debug -V
```

or

```
ctest -C Release -V
```

Building on Linux
-----------------

Dependencies:

- GNU C++ compiler
- CMake

Run CMake to generate a Makefile:

```
mkdir build
cd build
cmake ..
```

To build (in the build directory):

```
make
```

To run the tests (in the build directory):

```
ctest -V
```
