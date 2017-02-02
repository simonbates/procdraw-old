Procdraw C++ Coding Style
=========================

Layout
------

* Indentation is 4 spaces
* Use a maximum line length of 80 characters

Naming
------

* Types: PascalCase
* Functions and methods: PascalCase
* Public data members: PascalCase
* Variables and parameters: lowerCamelCase
* Private class data members: mCamelCase
* Output pointer parameters: outCamelCase

Conditional Compilation
-----------------------

* Avoid #ifdefs in .c or .cpp files
* Instead, put conditionals into header files and (if applicable)
  provide no-op functions or classes

Passing data to functions
-------------------------

| Mechanism | When |
|-----------|------|
| By value | Use for read-only primitive types |
| By reference | Use const references for passing read-only structs and classes |
| By smart pointer | Use for objects with automatic memory management |
| By pointer | Use pointers for the purpose of returning multiple values, prefixing each parameter name with "out" (but for a single return value, prefer return by value) |

Returning data from functions
-----------------------------

| Mechanism | When |
|-----------|------|
| By value | This is the preferred mechanism to use when returning a single value (including compound objects) |
| By reference | Use with care; a valid use case would be returning access to a collection member |
| By smart pointer | Use for objects with automatic memory management |
| By pointer | Use with care; for example to provide access to an object with program-lifetime temporal scope |

If a function has a single return value (including compound objects),
prefer return by value. If there are situations where there is evidence
(profiling) that return by value is too slow, then use a different
approach (such as pass modifiable object by pointer).
