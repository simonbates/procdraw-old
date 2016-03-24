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
- If a function has a single return value (including compound
  objects), use return by value. If there are situations where there
  is evidence (profiling) that return by value is too slow, then use a
  different approach. But return by value is clearer and C++ compilers
  do so much optimization that using a different approach, without
  evidence because it might be faster, is premature.

Pre-commit checklist
--------------------

- Run Astyle to format the source code
- Build and run the tests
- Run Procdraw
