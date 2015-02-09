(progn
  (ambient-light-color 0 0 (/ 7 10))
  (light-color 0 0 (/ 3 10))
  (color 40 (/ 3 10) (/ 6 10))
  (setq rot-x (saw))
  (setq rot-y (saw))
  (setq draw (lambda ()
    (progn
      (put rot-x 'freq
        (map-range 0 1 (/ 60) (/ -60) (mouse-y)))
      (put rot-y 'freq
        (map-range 0 1 (/ 60) (/ -60) (mouse-x)))
      (background 200 (/ 7 10) (/ 9 10))
      (rotate-y (map-range 0 1 0 (* 2 pi) (sigval rot-y)))
      (rotate-x (map-range 0 1 0 (* 2 pi) (sigval rot-x)))
      (tetrahedron)))))
