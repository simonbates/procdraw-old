(progn
  (ambient-light-color 0 0 (/ 7 10))
  (light-color 0 0 (/ 3 10))
  (color 40 (/ 3 10) (/ 6 10))
  (setq saw-x (saw))
  (setq saw-y (saw))
  (setq draw (lambda ()
    (progn
      (put saw-x 'freq (lerp (/ 60) (/ -60) (mouse-y)))
      (put saw-y 'freq (lerp (/ 60) (/ -60) (mouse-x)))
      (background 200 (/ 7 10) (/ 9 10))
      (rotate-y (sigval saw-y))
      (rotate-x (sigval saw-x))
      (tetrahedron)))))
