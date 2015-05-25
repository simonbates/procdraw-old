#include "procdraw_app_sdl.h"
#include "procdraw_app_sdl_lisp.h"
#include "sdl_util.h"
#include <SDL2/SDL.h>
#include <readline/readline.h>
#include <iostream>

namespace procdraw {

    static int CliThreadFunction(void *data);
    static void LineHandler(char *line);

    ProcDrawAppSdl::ProcDrawAppSdl() : quit_(false)
    {
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            ThrowSdlError();
        }

        renderer_ = std::unique_ptr<GlRenderer>(new GlRenderer());

        RegisterProcDrawAppSdlFunctionsForLisp(this, &L_);
        EvalExampleProg();
    }

    ProcDrawAppSdl::~ProcDrawAppSdl()
    {
        // TODO The constructor/destructor design here is not safe -- if the constructor throws an exception, the destructor will not be called
        SDL_Quit();
    }

    int ProcDrawAppSdl::Run()
    {
        SDL_Thread *cliThread = SDL_CreateThread(CliThreadFunction, "CliThread", this);

        MainLoop();

        int threadReturnValue;
        SDL_WaitThread(cliThread, &threadReturnValue);

        return 0;
    }

    bool ProcDrawAppSdl::IsQuit()
    {
        return quit_;
    }

    void ProcDrawAppSdl::MainLoop()
    {
        SDL_Event e;
        while (!quit_) {
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                    quit_ = true;
                }
            }
            L_.Apply("draw");
            renderer_->DoSwap();
        }
    }

    void ProcDrawAppSdl::EvalExampleProg()
    {
        auto prog = "(def draw ()"
                    "  (background 200 (/ 6 10) (/ 9 10))"
                    "  (rotate-z (mouse-x))"
                    "  (colour 9 (/ 7 10) (/ 7 10))"
                    "  (triangle)"
                    "  (colour 100 (/ 7 10) (/ 7 10))"
                    "  (point))";
        L_.Eval(L_.Read(prog));
    }

    static int CliThreadFunction(void *data)
    {
        ProcDrawAppSdl *app = reinterpret_cast<ProcDrawAppSdl*>(data);
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

        rl_callback_handler_remove();

        return 0;
    }

    static void LineHandler(char *line)
    {
        // TODO support quit with ^D by testing for line == NULL
        // TODO send line to the app for processing, rather than just echoing
        if (line != NULL) {
            std::cout << line << std::endl;
            free (line);
        }
    }

}
