(progn
  (setq mouse-angle (lambda (pos)
    (lerp (* 2 pi) (* -2 pi) pos)))

  (setq draw (lambda ()
    (progn
      (background 0 0 0)
      (rotate-x (mouse-angle (mouse-y)))
      (rotate-y (mouse-angle (mouse-x)))
      (tetrahedron)))))
