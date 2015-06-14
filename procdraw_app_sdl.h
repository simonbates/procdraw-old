#pragma once

#include "lisp_interpreter.h"
#include "glrenderer.h"
#include <memory>

namespace procdraw {

    class ReplThread;

    class ProcDrawAppSdl {
    public:
        ProcDrawAppSdl();
        ~ProcDrawAppSdl();
        inline GlRenderer* Renderer()
        {
            return &renderer_;
        }
        int MainLoop();
        bool IsQuit() const;
    private:
        bool quit_;
        GlRenderer renderer_;
        LispInterpreter L_;
        std::unique_ptr<ReplThread> repl_;
        void EvalExampleProg();
    };

}
