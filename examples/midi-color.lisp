(progn
  (ambient-light-color 0 0 (/ 4 5))
  (light-color 0 0 (/ 1 5))
  (setq running (toggle))
  (=> '(sigval key-space) running 'event)
  (setq angle-x (saw))
  (=> '(* (sigval running) (lerp (/ -120) (/ 120) (sigval midicc-1-1))) angle-x 'freq)
  (setq angle-y (saw))
  (=> '(* (sigval running) (lerp (/ -120) (/ 120) (sigval midicc-1-2))) angle-y 'freq)
  (def draw ()
    (background (lerp 0 360 (sigval midicc-1-17)) (sigval midicc-1-18) 1)
    (color (lerp 0 360 (- 1 (sigval midicc-1-17))) (sigval midicc-1-18) 1)
    (rotate-x (sigval angle-x))
    (rotate-y (sigval angle-y))
    (scale (lerp (/ 4) 4 (sigval midicc-1-19)))
    (cube)))
