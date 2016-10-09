#pragma once

#include "procdraw/graphics/glrenderer.h"
#include "procdraw/command_processor.h"
#include "procdraw/line_buffer.h"
#include PROCDRAW_SDL2_H
#include <string>
#include <vector>

namespace procdraw {

struct ConsoleLine
{
  std::string text;
  TextLayout<GLfloat> layout;
  explicit ConsoleLine(const std::string& text)
    : text(text)
  {
  }
};

class Console
{
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
