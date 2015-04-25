((nil . ((eval . (progn
                   (when (require 'projectile nil t)
                     (puthash (projectile-project-root)
                              "make -C build --no-print-directory"
                              projectile-compilation-cmd-map)
                     (puthash (projectile-project-root)
                              "make -C build --no-print-directory test"
                              projectile-test-cmd-map)))))))
