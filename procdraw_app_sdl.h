#pragma once

#include "lisp_interpreter.h"
#include "glrenderer.h"

namespace procdraw {

    class ProcDrawAppSdl {
    public:
        ProcDrawAppSdl();
        inline GlRenderer* Renderer()
        {
            return &renderer_;
        }
        int Run();
        bool IsQuit() const;
    private:
        bool quit_;
        LispInterpreter L_;
        GlRenderer renderer_;
        void MainLoop();
        void EvalExampleProg();
    };

}
