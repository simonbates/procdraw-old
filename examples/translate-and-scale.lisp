(setq draw (lambda ()
  (progn
    (background 0 0 0)
    (translate (lerp -4 4 (mouse-x)) 0 0)
    ((lambda (scale-val)
        (scale scale-val scale-val scale-val))
      (lerp 2 (/ 4) (mouse-y)))
    (rotate-y (lerp 0 (* 4 pi) (mouse-y)))
    (tetrahedron)))))
