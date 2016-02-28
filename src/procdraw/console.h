#pragma once

#include "glrenderer.h"
#include "line_buffer.h"
#include "command_processor.h"
#include <SDL2/SDL.h>
#include <vector>

namespace procdraw {

    class Console {
    public:
        Console(CommandProcessor *p) :
            cmdProcessor_(p), inputLine_(topLevelPrompt_) { }
        void Draw(GlRenderer *renderer);
        void InputText(char *text);
        void Println(const std::string &str);
        void ProcessKey(SDL_KeyboardEvent *key);
    private:
        std::string topLevelPrompt_ = "> ";
        std::string continuedLinePrompt_ = "... ";
        CommandProcessor *cmdProcessor_;
        bool continuedLine_ = false;
        std::string cmd_ = "";
        std::vector<std::string> lines_;
        LineBuffer inputLine_;
        void ProcessReturn();
    };

}
