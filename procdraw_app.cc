#include "procdraw_app.h"
#include "procdraw_app_lisp.h"
#include <SDL2/SDL.h>

namespace procdraw {

    ProcdrawApp::ProcdrawApp()
    {
        RegisterProcdrawAppFunctions(this, &L_);
        EvalExampleProg();
        cli_ = std::unique_ptr<CLI>(new CLI(this));
    }

    int ProcdrawApp::MainLoop()
    {
        bool quit = false;
        SDL_Event e;
        while (!quit) {
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                }
            }
            cli_->Poll();
            L_.Apply("draw");
            renderer_.DoSwap();
        }
        return 0;
    }

    std::string ProcdrawApp::Eval(const std::string &expr)
    {
        std::string val;
        try {
            val = L_.PrintString(L_.Eval(L_.Read(expr)));
        }
        catch (std::exception e) {
            val = e.what();
        }
        return val;
    }

    void ProcdrawApp::EvalExampleProg()
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
