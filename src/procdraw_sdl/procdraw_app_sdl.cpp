#include "procdraw_app_sdl.h"
#include "procdraw_app_sdl_lisp.h"
#include "sdl_util.h"
#include <SDL2/SDL.h>

namespace procdraw {

    ProcDrawAppSdl::ProcDrawAppSdl()
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
        SDL_Quit();
    }

    int ProcDrawAppSdl::MainLoop()
    {
        SDL_Event e;
        bool quit = false;

        while (!quit) {
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                }
            }
            L_.Apply("draw");
            renderer_->DoSwap();
        }

        return 0;
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
