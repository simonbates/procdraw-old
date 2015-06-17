#pragma once

#include "glrenderer.h"
#include "lisp_interpreter.h"
#include "cli.h"
#include <memory>
#include <string>

namespace procdraw {

    class ProcdrawApp {
    public:
        ProcdrawApp();
        inline GlRenderer* Renderer()
        {
            return &renderer_;
        }
        int MainLoop();
        std::string Eval(const std::string &expr);
    private:
        GlRenderer renderer_;
        LispInterpreter L_;
        std::unique_ptr<CLI> cli_;
        void EvalExampleProg();
    };

}
