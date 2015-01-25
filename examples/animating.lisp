(progn
  (setq rotation-x-angle 0)
  (setq rotation-y-angle 0)
  (light-color 0 0 (/ 3 10))
  (ambient-light-color 0 0 (/ 7 10))
  (color 40 (/ 3 10) (/ 6 10))
  (setq draw (lambda ()
    (progn
      (setq rotation-x-velocity
        (map-range 0 (height) (/ pi 60) (/ pi -60) (mouse-y)))
      (setq rotation-y-velocity
        (map-range 0 (width) (/ pi 60) (/ pi -60) (mouse-x)))
      (setq rotation-x-angle
        (wrap-range 0 (* 2 pi) (+ rotation-x-angle rotation-x-velocity)))
      (setq rotation-y-angle
        (wrap-range 0 (* 2 pi) (+ rotation-y-angle rotation-y-velocity)))
      (background 200 (/ 7 10) (/ 9 10))
      (rotate-y rotation-y-angle)
      (rotate-x rotation-x-angle)
      (tetrahedron)))))
