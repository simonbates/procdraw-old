Procdraw
========

[![Build Status](https://travis-ci.org/simonbates/procdraw.svg?branch=master)](https://travis-ci.org/simonbates/procdraw)

Procdraw is/will be a platform for exploring programming language
design and interaction design for live programming.

In its current form, it consists of:

* An interpreter for a Lisp-like language with:
    * s-expression syntax
    * list-structured memory
    * procedures stored as list structures and always recoverable and
      modifiable at run time
* A dataflow model ("signals")
* The beginnings of a 3D graphics API using OpenGL

It runs on Linux and Windows (the Windows port is still a little rough).

I'm still working on the basics at the moment: memory model,
interpreters, editing interfaces. Once I'm a little further along, I'd
like to use Procdraw to explore:

* Systems, interfaces, and notations for experimental modification of
  running systems ('circuit bending of code', towers of interpreters,
  self-modifying code, evolutionary algorithms and "artist as
  gardener")
* Designing a runtime environment that trys to continue running in the
  presence of 'errors' -- potentially providing a result or behaviour
  even if it isn't 'correct'
* Coordination of different interfaces/notations for inspection and
  editing such as using both direct manipulation and text-based
  notations
* Runtime modification of as much as possible, including the Procdraw
  user interfaces

Building and running Procdraw on Linux
--------------------------------------

### Install dependencies

Procdraw has the following build dependencies:

* OpenGL 3.2
* GNU C++ compiler
* CMake
* SDL2
* ALSA (for MIDI input)

Optional dependencies:

* clang-format: for code formatting
* Python 2: for running cpplint
* Jing: to validate the XML function-specs

### Configure the font

Edit the file `src/procdraw_core/graphics/font_config.h` and set
`MonospaceFontFilePath` to a suitable monospaced font on your system.

### Build Procdraw and run the tests

Procdraw uses the CMake build system. To build Procdraw and run the
tests:

```
$ git submodule update --init
$ mkdir build
$ cd build
$ cmake ..
$ make
$ ctest -V
```

To make a debug build, invoke CMake with the `-DCMAKE_BUILD_TYPE=Debug` option:

```
$ cmake -DCMAKE_BUILD_TYPE=Debug ..
```

### Running Procdraw

Procdaw can be run from the build directory:

```
$ src/procdraw
```

Building and running Procdraw on Windows
----------------------------------------

### Install dependencies

Procdraw has the following build dependencies:

* OpenGL 3.2
* Visual Studio Community 2015 (with C++ tools and Windows SDK)
* CMake
* SDL2

SDL2 can be downloaded from the [SDL
website](https://www.libsdl.org/). Download the development libraries
for Windows Visual C++ and unpack to your filesystem. To tell Procdraw
where to find the SDL2 files, please set an environment variable,
`PROCDRAW_SDL2_DIR`, to the location that the distribution was
unpacked to (such as `C:/SDL/SDL2-2.0.4`).

### Configure the font

Edit the file `src/procdraw_core/graphics/font_config.h` and set
`MonospaceFontFilePath` to a suitable monospaced font on your system
(such as `"C:/Windows/Fonts/consola.ttf"`).

### Build Procdraw and run the tests

Run CMake to generate the Visual Studio solution:

```
> git submodule update --init
> mkdir build
> cd build
> cmake -G "Visual Studio 14" ..
```

To build in Visual Studio:

- Open `build\Procdraw.sln`
- Build Solution (Ctrl+Shift+B)

To run the tests in Visual Studio:

- Build the `RUN_TESTS` project

### Running Procdraw

In Visual Studio, start a new instance of the `procdraw_app` project.

License
-------

MIT (c) Simon Bates
