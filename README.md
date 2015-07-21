Procdraw
========

Procdraw is/will be a platform for exploring programming language and
interaction design for live programming.

In its current form, it consists of:

* An interpreter for a Lisp-like language with:
    * s-expression syntax
    * list-structured memory
    * code is stored as list structures and is always recoverable and
      modifiable at run time
    * a simple object model ("tables")
* A simple data-flow model ("signals")
* The beginnings of a 3D graphics API using OpenGL

See: [Documentation/lang.md](Documentation/lang.md)

Building on Linux
-----------------

Dependencies:

* CMake
* GNU C++ compiler
* GNU Readline
* SDL2
* GLEW
* GLM

You can build Procdraw and run the tests with these steps:

```
$ mkdir build
$ cd build
$ cmake ..
$ make
$ ctest --output-on-failure
```

Running Procdraw from Emacs
---------------------------

Configure Procdraw as the inferior lisp program:

```
(setq inferior-lisp-program "/path/to/procdraw")
```

Procdraw can then be started from Emacs with:

```
M-x run-lisp
```

When expressions are evaluated (C-x C-e) in Lisp mode, they will be
sent to Procdraw. To make a Lisp buffer, you can open a Lisp file, or
create a new buffer with:

```
C-x b BUFFER-NAME (such as *procdraw*)
M-x lisp-mode
```
