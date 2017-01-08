#pragma once

#include "procdraw/core/command_processor.h"
#include "procdraw/core/line_buffer.h"
#include "procdraw/opengl/console_line.h"
#include "procdraw/opengl/glrenderer.h"
#include PROCDRAW_SDL2_H
#include <string>
#include <vector>

namespace procdraw {

class Console {
public:
    Console(GlRenderer* r, CommandProcessor* p, int width, int height);
    void Draw();
    void Println(const std::string& str);
    void InputText(char* text);
    void ProcessKey(SDL_KeyboardEvent* key);

private:
    GlRenderer* renderer_;
    std::string topLevelPrompt_ = "> ";
    std::string continuedLinePrompt_ = "... ";
    CommandProcessor* cmdProcessor_;
    int consoleWidth_ = 0;
    int consoleHeight_ = 0;
    int linespace_ = 0;
    int lineCapacity_ = 0;
    int numVisibleLines_ = 0;
    std::size_t displayFromLogicalLine_ = 0;
    std::size_t displayFromPhysicalLine_ = 0;
    bool continuedLine_ = false;
    std::string cmd_ = "";
    std::vector<ConsoleLine> lines_;
    LineBuffer inputLine_;
    TextLayout<GLfloat> inputLineLayout_;
    bool inputLineNeedsLayOut_ = true;
    void ProcessReturn();
    void DrawConsoleText();
    void ScrollShowInput(int inputNumLines);
};
}
