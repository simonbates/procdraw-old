#include "procdraw_app.h"
#include "procdraw_app_lisp.h"
#include "signals.h"
#include <SDL2/SDL.h>

namespace procdraw {

    ProcdrawApp::ProcdrawApp()
    {
        RegisterSignals(&L_);
        RegisterProcdrawAppFunctions(this, &L_);
        cli_ = std::unique_ptr<CLI>(new CLI(this));
    }

    int ProcdrawApp::MainLoop()
    {
        bool quit = false;

        auto s_show_repl = L_.SymbolRef("show-repl");
        L_.Set(s_show_repl, L_.False, L_.Nil);

        SDL_Event e;
        while (!quit) {
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                }
                else if (e.type == SDL_KEYDOWN) {
                    if ((e.key.keysym.sym == SDLK_ESCAPE) && (e.key.repeat == 0)) {
                        // TODO L_.Set(s_show_repl, L_.Not(L_.Eval(s_show_repl)), L_.Nil)
                        if (L_.BoolVal(L_.Eval(s_show_repl))) {
                            L_.Set(s_show_repl, L_.False, L_.Nil);
                        }
                        else {
                            L_.Set(s_show_repl, L_.True, L_.Nil);
                        }
                    }
                }
            }
            cli_->Poll();
            L_.Call("clear-stepped-signals");
            L_.Call("draw");
            if (L_.BoolVal(L_.Eval(s_show_repl))) {
                renderer_.Begin2D();
                renderer_.Rect(40, 80, 280, 240);
                renderer_.End2D();
            }
            renderer_.DoSwap();
            frameCounter_.RecordFrame();
        }
        return 0;
    }

    std::string ProcdrawApp::DoCommand(const std::string &cmd)
    {
        std::string val;
        try {
            val = L_.PrintToString(L_.Eval(L_.Read(cmd)));
        }
        catch (std::exception e) {
            val = e.what();
        }
        return val;
    }

    BalancedState ProcdrawApp::CheckCommand(const std::string &cmd)
    {
        return L_.CheckBalanced(cmd);
    }

    double ProcdrawApp::FramesPerSecond()
    {
        return frameCounter_.GetFramesPerSecond();
    }

}
