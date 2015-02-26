Contributing
============

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
- Use UTF-16 when interfacing with the Windows API and convert to/from UTF-8 as needed

Pre-commit checklist
--------------------

- Build and run the tests on Windows
- Build and run the tests on Linux
- Run ProcDraw and verify the examples
- Run Astyle to format the source code
