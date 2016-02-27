#pragma once

#include "glrenderer.h"
#include "line_buffer.h"
#include <SDL2/SDL.h>

namespace procdraw {

    class Console {
    public:
        Console() : line_("> ") { }
        void Draw(GlRenderer *renderer);
        void InputText(char *text);
        void ProcessKey(SDL_KeyboardEvent *key);
    private:
        LineBuffer line_;
    };

}
