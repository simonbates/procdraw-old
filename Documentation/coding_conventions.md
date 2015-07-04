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
- Order of header #includes: for this cpp file, within project, system
- Use RAII (Resource Acquisition Is Initialization)
- For text, use UTF-8 in std::string

Pre-commit checklist
--------------------

- Run Astyle to format the source code
- Build and run the tests
- Run Procdraw
