#pragma once

#include "glrenderer.h"
#include "line_buffer.h"
#include "command_processor.h"
#include <SDL2/SDL.h>
#include <vector>

namespace procdraw {

    struct ConsoleLine {
        std::string text;
        TextLayout<GLfloat> layout;
        ConsoleLine(const std::string &text) : text(text) { }
    };

    class Console {
    public:
        Console(GlRenderer *r, CommandProcessor *p) :
            renderer_(r), cmdProcessor_(p), inputLine_(topLevelPrompt_) { }
        void Draw();
        void InputText(char *text);
        void Println(const std::string &str);
        void ProcessKey(SDL_KeyboardEvent *key);
    private:
        GlRenderer *renderer_;
        std::string topLevelPrompt_ = "> ";
        std::string continuedLinePrompt_ = "... ";
        CommandProcessor *cmdProcessor_;
        bool continuedLine_ = false;
        std::string cmd_ = "";
        std::vector<ConsoleLine> lines_;
        LineBuffer inputLine_;
        TextLayout<GLfloat> inputLineLayout_;
        bool inputLineNeedsLayout_ = true;
        void ProcessReturn();
    };

}
