(progn
  (setq saw1 (saw))
  (setq sin1 (sin-osc))
  (put-slot sin1 'freq (/ (* 11 60)))
  (=> sin1 saw1 'freq (lambda (freq) (lerp (/ (* 7 60)) (/ 90) freq)))
  (def draw ()
    (background 200 (/ 6 10) (/ 9 10))
    (rotate-y (sigval saw1))
    (translate 3 0 0)
    (rotate-y (sigval saw1))
    (color (lerp 5 9 (sigval sin1))
           (lerp (/ 6 10) (/ 8 10) (sigval sin1))
           (lerp (/ 6 10) (/ 8 10) (sigval sin1)))
    (tetrahedron)))
