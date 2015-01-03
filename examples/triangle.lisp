(setq draw (lambda ()
  (progn
    (background 0 0 0)
    (rotate-z (/ (* (mouse-x) pi) (width)))
    (triangle))))
