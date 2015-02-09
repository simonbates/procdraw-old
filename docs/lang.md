Language
========

Types
-----

- Numbers
- Booleans
- Symbols
- Strings
- Pairs and lists
- Tables
- CFunctions

Special-forms and functions
---------------------------

- `(* ...)`
- `(+ ...)`
- `(- number ...)`
- `(/ number ...)`
- `(apply function argument-list)`
- `(car obj)`
- `(cdr obj)`
- `(eq obj1 obj2)`
- `(if test then else)`
- `(lambda arg-list body)`
- `(lerp start stop val)`
- `(map-range start1 stop1 start2 stop2 val)`
- `(progn action ...)`
- `(quote arg)`
- `(setq identifier value)`
- `(wrap start stop val)`

Tables
------

- `(make-table)`
- `(get table key)`
- `(put table key val)`
- `(clear table)`
- `(key table ...)`

Signals
-------

- `(make-signal stepfun)`
- `('step signal)`
- `(sigval signal)`
- `(saw)`

Graphics
--------

- `(ambient-light-color h s v)`
- `(background h s v)`
- `(color h s v)`
- `(frames-per-second)`
- `(height)`
- `(light-color h s v)`
- `(mouse-x)`
- `(mouse-y)`
- `(rotate-x angle)`
- `(rotate-y angle)`
- `(rotate-z angle)`
- `(scale x y z)`
- `(tetrahedron)`
- `(translate x y z)`
- `(width)`
