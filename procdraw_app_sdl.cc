#include "procdraw_app_sdl.h"
#include "procdraw_app_sdl_lisp.h"
#include "repl.h"
#include <SDL2/SDL.h>

namespace procdraw {

    ProcDrawAppSdl::ProcDrawAppSdl() : quit_(false)
    {
        RegisterProcDrawAppSdlFunctions(this, &L_);
        EvalExampleProg();
        repl_ = std::unique_ptr<ReplThread>(new ReplThread("repl", this));
    }

    ProcDrawAppSdl::~ProcDrawAppSdl()
    {
        // Signal to other threads that we are shutting down
        quit_ = true;
    }

    int ProcDrawAppSdl::MainLoop()
    {
        SDL_Event e;
        while (!quit_) {
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                    quit_ = true;
                }
            }
            L_.Apply("draw");
            renderer_.DoSwap();
        }
        return 0;
    }

    bool ProcDrawAppSdl::IsQuit() const
    {
        return quit_;
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

}
