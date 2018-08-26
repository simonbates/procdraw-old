# Procdraw C++ Coding Style

## Naming

- Types: PascalCase
- Functions and methods: PascalCase
- Variables and parameters: lowerCamelCase
- Class data members: lowerCamelCaseUnderscore_
- Struct data members: lowerCamelCase

## Conditional Compilation

- Avoid `#ifdefs` in `.c` or `.cpp` files
- Instead, put conditionals into header files and (if applicable)
  provide no-op functions or classes

## C++ Core Guidelines

- http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines
