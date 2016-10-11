#include "procdraw_core/console.h"
#include <algorithm>
#include <string>

namespace procdraw {

Console::Console(GlRenderer* r, CommandProcessor* p, int width, int height)
  : renderer_(r)
  , cmdProcessor_(p)
  , consoleWidth_(width)
  , consoleHeight_(height)
  , inputLine_(topLevelPrompt_)
{
  linespace_ = r->GetLinespace();
  // In C++ 11, the result of integer division is truncated towards zero
  lineCapacity_ = height / linespace_;
}

void
Console::Draw()
{
  // Update the input line layout if needed
  if (inputLineNeedsLayOut_) {
    inputLineLayout_ =
      renderer_->LayOutText(inputLine_.GetLine(), consoleWidth_);
    inputLineNeedsLayOut_ = false;
  }

  // Draw the console background
  renderer_->Begin2D();
  renderer_->Color(0, 0, 0, 0.6f);
  renderer_->Rect(0, 0, consoleWidth_, consoleHeight_);

  // Calculate the cursor position
  // TODO: Don't need to calculate the cursor pos each time, only when changes
  int cursorX, cursorY, cursorWidth, cursorHeight;
  inputLineLayout_.CalculateFixedWidthBlockCursorPos(
    inputLine_.GetCursorPos(), &cursorX, &cursorY, &cursorWidth, &cursorHeight);

  // Scroll if needed
  ScrollShowInput(inputLineLayout_.NumLines());

  // Adjust the cursorY position for numVisibleLines_
  cursorY += linespace_ * numVisibleLines_;

  // Draw the cursor background
  renderer_->DrawBlockCursorBackground(cursorX, cursorY, cursorWidth,
                                       cursorHeight);

  // Draw text
  renderer_->BeginText();
  DrawConsoleText();

  // Do block cursor inversion
  renderer_->Begin2D();
  renderer_->DrawBlockCursorInversion(cursorX, cursorY, cursorWidth,
                                      cursorHeight);

  renderer_->Begin3D();
}

void
Console::Println(const std::string& str)
{
  ConsoleLine line(str);
  line.layout = renderer_->LayOutText(str, consoleWidth_);
  lines_.push_back(line);
  numVisibleLines_ += line.layout.NumLines();
}

void
Console::InputText(char* text)
{
  inputLine_.Insert(text[0]);
  inputLineNeedsLayOut_ = true;
}

void
Console::ProcessKey(SDL_KeyboardEvent* key)
{
  switch (key->keysym.sym) {
    case SDLK_BACKSPACE:
      inputLine_.DeleteBack();
      inputLineNeedsLayOut_ = true;
      break;
    case SDLK_LEFT:
      inputLine_.BackwardChar();
      inputLineNeedsLayOut_ = true;
      break;
    case SDLK_RIGHT:
      inputLine_.ForwardChar();
      inputLineNeedsLayOut_ = true;
      break;
    case SDLK_RETURN:
      Println(inputLine_.GetLine());
      ProcessReturn();
      inputLine_.Clear(continuedLine_ ? continuedLinePrompt_ : topLevelPrompt_);
      inputLineNeedsLayOut_ = true;
      break;
  }
}

void
Console::ProcessReturn()
{
  cmd_ += "\n";
  cmd_ += inputLine_.GetUserText();
  if (std::all_of(cmd_.begin(), cmd_.end(), isspace)) {
    // If cmd_ is all white space, ignore it
    cmd_ = "";
    continuedLine_ = false;
  } else {
    switch (cmdProcessor_->CheckCommand(cmd_)) {
      case BalancedState::Balanced:
        Println(cmdProcessor_->DoCommand(cmd_));
        cmd_ = "";
        continuedLine_ = false;
        break;
      case BalancedState::TooManyClosingParens:
        Println("Too many closing parens");
        cmd_ = "";
        continuedLine_ = false;
        break;
      case BalancedState::NotClosed:
        continuedLine_ = true;
        break;
    }
  }
}

void
Console::DrawConsoleText()
{
  int y = 0;
  for (decltype(lines_)::size_type i = displayFromLogicalLine_;
       i < lines_.size(); ++i) {
    auto line = lines_.at(i);
    auto startLineNum =
      (i == displayFromLogicalLine_ ? displayFromPhysicalLine_ : 0);
    auto endLineNum = line.layout.NumLines();
    renderer_->DrawText(0, y, line.layout, startLineNum, endLineNum);
    y += linespace_ * (endLineNum - startLineNum);
  }
  renderer_->DrawText(0, y, inputLineLayout_, 0, inputLineLayout_.NumLines());
}

void
Console::ScrollShowInput(int inputNumLines)
{
  if (numVisibleLines_ + inputNumLines > lineCapacity_) {
    // TODO: Handle case where the inputNumLines > lineCapacity_
    while (numVisibleLines_ > lineCapacity_ - inputNumLines) {
      if (displayFromPhysicalLine_ + 1 <
          lines_.at(displayFromLogicalLine_).layout.NumLines()) {
        ++displayFromPhysicalLine_;
        --numVisibleLines_;
      } else if (displayFromLogicalLine_ + 1 < lines_.size()) {
        ++displayFromLogicalLine_;
        displayFromPhysicalLine_ = 0;
        --numVisibleLines_;
      } else {
        break;
      }
    }
  }
}
}
