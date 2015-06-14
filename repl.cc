#include "repl.h"
#include <readline/readline.h>
#include <iostream>

namespace procdraw {

    static int ReplThreadFunction(void *data);
    static void LineHandler(char *line);

    ReplThread::ReplThread(const char *name, ProcDrawAppSdl *app)
    {
        thread_ = SDL_CreateThread(ReplThreadFunction, name, app);
    }

    ReplThread::~ReplThread()
    {
        int status;
        SDL_WaitThread(thread_, &status);
        rl_callback_handler_remove();
    }

    static int ReplThreadFunction(void *data)
    {
        auto app = static_cast<ProcDrawAppSdl*>(data);
        fd_set fds;
        struct timeval timeout;

        timeout.tv_sec = 0;
        timeout.tv_usec = 10000;

        rl_callback_handler_install("> ", LineHandler);

        while (!app->IsQuit()) {
            FD_ZERO(&fds);
            FD_SET(fileno(rl_instream), &fds);

            int ready = select(FD_SETSIZE, &fds, NULL, NULL, &timeout);
            if (ready < 0) {
                perror("select");
                break;
            }

            if (FD_ISSET(fileno(rl_instream), &fds)) {
                rl_callback_read_char();
            }
        }

        return 0;
    }

    static void LineHandler(char *line)
    {
        // TODO support quit with ^D by testing for line == NULL
        // TODO send line to the app for processing, rather than just echoing
        // TODO ensure that any methods called on app from this thread are thread-safe
        if (line != NULL) {
            std::cout << "***" << line << "***" << std::endl;
            free(line);
        }
    }

}
