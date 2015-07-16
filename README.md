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

Build Procdraw and run the tests with these steps:

```
$ mkdir build
$ cd build
$ cmake ..
$ make
$ ctest --output-on-failure
```

Running procdraw from Emacs
---------------------------

Configure procdraw as the inferior lisp program:

```
(setq inferior-lisp-program "/path/to/procdraw")
```

Procdraw can then be started from Emacs with:

```
M-x run-lisp
```

When expressions are evaluated in Lisp major mode buffers, they will
be sent to procdraw. To make a Lisp buffer, open a Lisp file, or
create a new buffer with:

```
C-x b BUFFER-NAME (such as *procdraw*)
M-x lisp-mode
```
