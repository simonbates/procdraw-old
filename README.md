Building on Linux
-----------------

Dependencies:

- CMake
- GNU C++ compiler
- GNU Readline
- SDL2
- GLEW
- GLM

Run CMake to generate a Makefile:

```
> mkdir build
> cd build
> cmake ..
```

To build (in the `build` directory):

```
> make
```

To run the tests (in the `build` directory):

```
> ctest --output-on-failure
```
