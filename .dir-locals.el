((nil . ((eval . (progn
                   (when (require 'projectile nil t)
                     (puthash (projectile-project-root)
                              "cd build && make"
                              projectile-compilation-cmd-map)
                     (puthash (projectile-project-root)
                              "cd build && ctest --output-on-failure"
                              projectile-test-cmd-map))))))
 (c++-mode  . ((tab-width . 8) (indent-tabs-mode . nil) (c-basic-offset . 2)))
 (c-mode    . ((tab-width . 8) (indent-tabs-mode . nil) (c-basic-offset . 2)))
 (html-mode . ((sgml-basic-offset . 2))))
