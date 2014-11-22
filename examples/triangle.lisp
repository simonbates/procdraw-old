(setq draw (lambda ()
  (progn
    (background 0 0 0)
    (rotate (quotient (product (mousex) pi) (width)))
    (triangle))))
