Procdraw C++ Coding Style
=========================

Layout
------

- Indentation size: 4 spaces
- Maximum line length: 80 characters

Naming
------

- Types: PascalCase
- Functions and methods: lower_case
- Public data members: lower_case
- Variables and parameters: lower_case
- Private class data members: m_lower_case
- Static class data members: s_lower_case

Conditional Compilation
-----------------------

- Avoid #ifdefs in .c or .cpp files
- Instead, put conditionals into header files and (if applicable)
  provide no-op functions or classes

C++ Core Guidelines
-------------------

- http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines
