#include "procdraw_app_sdl.h"
#include "sdl_util.h"
#include <SDL2/SDL.h>

namespace procdraw {

    ProcDrawAppSdl::ProcDrawAppSdl()
    {
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            ThrowSdlError();
        }

        renderer_ = std::unique_ptr<GlRenderer>(new GlRenderer());
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
            Draw();
            renderer_->DoSwap();
        }

        return 0;
    }

    void ProcDrawAppSdl::Draw()
    {
        renderer_->Background(200.0f, 0.6f, 0.9f);
        renderer_->RotateZ(renderer_->MouseX());
        renderer_->Colour(9.0f, 0.7f, 0.7f);
        renderer_->Triangle();
        renderer_->Colour(100.0f, 0.7f, 0.7f);
        renderer_->Point();
    }

}
