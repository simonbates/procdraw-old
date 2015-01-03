(progn
  (setq mouse-rotate (lambda (pos)
      (+ (/ (* pos -4 pi) (width)) (* 2 pi))))

  (setq draw (lambda ()
    (progn
      (background 0 0 0)
      (rotate-x (mouse-rotate (mouse-y)))
      (rotate-y (mouse-rotate (mouse-x)))
      (tetrahedron)))))
