#include "procdraw_app.h"
#include "procdraw_app_lisp.h"
#include <SDL2/SDL.h>

namespace procdraw {

    ProcdrawApp::ProcdrawApp()
    {
        RegisterProcdrawAppFunctions(this, &L_);
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

    std::string ProcdrawApp::DoCommand(const std::string &cmd)
    {
        std::string val;
        try {
            val = L_.PrintString(L_.Eval(L_.Read(cmd)));
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

}
