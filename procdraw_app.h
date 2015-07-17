#pragma once

#include "glrenderer.h"
#include "lisp_interpreter.h"
#include "cli.h"
#include "command_processor.h"
#include "frame_counter.h"
#include <memory>
#include <string>

namespace procdraw {

    class ProcdrawApp : public CommandProcessor {
    public:
        ProcdrawApp();
        inline GlRenderer* Renderer()
        {
            return &renderer_;
        }
        int MainLoop();
        std::string DoCommand(const std::string &cmd);
        BalancedState CheckCommand(const std::string &cmd);
        double FramesPerSecond();
    private:
        GlRenderer renderer_;
        LispInterpreter L_;
        std::unique_ptr<CLI> cli_;
        FrameCounter frameCounter_;
    };

}
