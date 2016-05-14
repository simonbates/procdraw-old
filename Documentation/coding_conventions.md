Coding
======

C++ Conventions
---------------

- Stroustrup indentation and brace style
- Indentation is 4 spaces
- Always use brace-delimited blocks in control structures
- else is not cuddled
- Types, functions, and methods are in PascalCase
- Local variables and parameters are in lowerCamelCase
- Private member variables are in lowerCamelCase_ with a trailing underscore
- Prefix parameter names for output objects passed by pointer or
  reference with "out_"
- When splitting expressions across lines, break before a binary operator
- Order of header #includes: for this cpp file, within project, system
- Use RAII (Resource Acquisition Is Initialization)
- For text, use UTF-8 in std::string

Passing data to functions
-------------------------

| Mechanism | When |
|-----------|------|
| By value | Use for read-only primitive types |
| By reference | Use const references for passing read-only structs and classes; when providing references to data for the purpose of returning multiple values, prefix parameter names with "out_" (but for a single return value, prefer return by value) |
| By smart pointer | Use for objects with automatic memory management |
| By pointer | Prefer pass by reference instead; use pointers when compatibility with C is needed |

Returning data from functions
-----------------------------

| Mechanism | When |
|-----------|------|
| By value | This is the prefered mechanism to use when returning a single value (including compound objects) |
| By reference | Use with care; a valid use case would be returning access to a collection member |
| By smart pointer | Use for objects with automatic memory management |
| By pointer | Use with care; for example to provide access to an object with program-lifetime temporal scope |

If a function has a single return value (including compound objects),
prefer return by value. If there are situations where there is
evidence (profiling) that return by value is too slow, then use a
different approach (such as pass modifiable object by reference).

Pre-commit checklist
--------------------

- Run Astyle to format the source code
- Build and run the tests
- Run Procdraw
