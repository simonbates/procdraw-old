(setq draw (lambda ()
  (progn
    (background 0 0 0)
    (translate (map-range 0 (width) -4 4 (mouse-x)) 0 0)
    ((lambda (scale-val)
        (scale scale-val scale-val scale-val))
      (map-range 0 (height) 2 (/ 4) (mouse-y)))
    (rotate-y (map-range 0 (height) 0 (* 4 pi) (mouse-y)))
    (tetrahedron)))))
