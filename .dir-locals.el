((nil . ((eval . (progn
                   (when (require 'projectile nil t)
                     (puthash (projectile-project-root)
                              "cd build && make"
                              projectile-compilation-cmd-map)
                     (puthash (projectile-project-root)
                              "cd build && ctest --output-on-failure"
                              projectile-test-cmd-map)))))))
