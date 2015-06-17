#include "cli.h"
#include "procdraw_app.h"
#include <readline/readline.h>
#include <iostream>

namespace procdraw {

    static ProcdrawApp *app = NULL;

    static void LineHandler(char *line)
    {
        // TODO support quit with ^D by testing for line == NULL
        if (line != NULL) {
            std::string expr(line);
            free(line);
            std::cout << app->Eval(expr) << std::endl;
        }
    }

    CLI::CLI(ProcdrawApp *a)
    {
        app = a;
        rl_callback_handler_install("> ", LineHandler);
    }

    CLI::~CLI()
    {
        rl_callback_handler_remove();
    }

    void CLI::Poll()
    {
        fd_set fds;
        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 0;
        int rl_instream_no = fileno(rl_instream);

        bool more = true;
        while (more) {
            FD_ZERO(&fds);
            FD_SET(rl_instream_no, &fds);

            int ready = select(rl_instream_no + 1, &fds, NULL, NULL, &timeout);

            if (ready < 0) {
                perror("select");
                break;
            }

            if ((ready > 0) && FD_ISSET(rl_instream_no, &fds)) {
                rl_callback_read_char();
            }
            else {
                more = false;
            }
        }
    }

}
