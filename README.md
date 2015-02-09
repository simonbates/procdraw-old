ProcDraw
========

ProcDraw is a work-in-progress live programming system.

It consists of:

- a cross-platform core with an embeddable interpreter for a Lisp-like language
- a GUI and graphics engine for Windows

Videos: https://vimeo.com/channels/858277

Language
--------

ProcDraw includes an interpreter for a Lisp-like language with:

- s-expression syntax
- list structured memory
- code is stored as list structures and is always recoverable and modifiable at runtime

See: [docs/lang.md](docs/lang.md)

Source code layout
------------------

- `src/procdraw_lib` - platform-independent code
- `src/procdraw_win` - Windows GUI code
- `src/procdraw_winmain` - Windows GUI WinMain
- `src/procrepl` - a REPL
- `tests/procdraw_lib_tests` - tests

Building on Windows
-------------------

Dependencies:

- Visual Studio Community 2013
- CMake

Run CMake to generate the Visual Studio solution:

```
> mkdir build
> cd build
> cmake -G "Visual Studio 12" ..
```

To build in Visual Studio:

- Open `build\ProcDraw.sln`
- Build Solution (Ctrl+Shift+B)

To run ProcDraw from Visual Studio:

- Set `procdraw_winmain` as the Startup Project
- Set the `procdraw_winmain` project Working Directory to the `procdraw` directory (Properties/Debugging/Working Directory)
- Start Debugging (F5) or Start Without Debugging (Ctrl+F5)

To run the tests in Visual Studio:

- Build the `test` project

Building on Linux
-----------------

The ProcDraw GUI and graphics are Windows-only but the language core and other platform-independent code can be built on Linux (and other platforms too hopefully, but I only test Linux).

Dependencies:

- GNU C++ compiler
- CMake

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
> make test
```

### Testing the Linux build with Vagrant

```
> cd tools\vagrantfiles\trusty64
> vagrant up
> vagrant ssh -c ./make-test-procdraw.sh
> vagrant halt
```
