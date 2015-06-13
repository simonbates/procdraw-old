Procdraw
========

Building on Linux
-----------------

Dependencies:

- CMake
- GNU C++ compiler
- GNU Readline
- SDL2
- GLEW
- GLM

You can build Procdraw and run the tests with these steps:

```
$ mkdir build
$ cd build
$ cmake ..
$ make
$ ctest --output-on-failure
```
