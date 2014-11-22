(setq draw (lambda ()
  (progn
    (background 0 0 0)
    (rotate (/ (* (mousex) pi) (width)))
    (triangle))))
