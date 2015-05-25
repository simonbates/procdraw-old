#pragma once

#include "glrenderer.h"
#include "lisp_interpreter.h"
#include <memory>

namespace procdraw {

    class ProcDrawAppSdl {
    public:
        ProcDrawAppSdl();
        ~ProcDrawAppSdl();
        inline GlRenderer* Renderer()
        {
            return renderer_.get();
        }
        int Run();
        bool IsQuit();
    private:
        bool quit_;
        std::unique_ptr<GlRenderer> renderer_;
        LispInterpreter L_;
        void MainLoop();
        void EvalExampleProg();
    };

}
