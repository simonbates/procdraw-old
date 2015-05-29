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
- `(def function-name arg-list body)`
- `(eq obj1 obj2)`
- `(if test then else)`
- `(lambda arg-list body)`
- `(lerp start stop val)`
- `(map-range start1 stop1 start2 stop2 val)`
- `(norm start stop val)`
- `(progn action ...)`
- `(quote arg)`
- `(setq identifier value)`
- `(wrap start stop val)`

Tables
------

- `(make-table)`
- `(get table key)`
- `(put table key val)`
- `(keys table)`
- `(clear table)`
- `(key table ...)`

Signals
-------

- `(make-signal stepfun)`
- `(=> source dest-signal dest-key &optional mapfun)`
- `('step signal)`
- `(sigval signal)`
- `(saw)`
- `(sin-osc)`
- `(tri)`

Graphics
--------

- `(ambient-light-color h s v)`
- `(background h s v)`
- `(color h s v)`
- `(cube)`
- `(frames-per-second)`
- `(height)`
- `(light-color h s v)`
- `(mouse-x)`
- `(mouse-y)`
- `(rotate-x turns)`
- `(rotate-y turns)`
- `(rotate-z turns)`
- `(scale x y z)`
- `(tetrahedron)`
- `(translate x y z)`
- `(width)`
