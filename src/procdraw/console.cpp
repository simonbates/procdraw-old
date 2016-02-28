#include "console.h"

namespace procdraw {

    void Console::Draw(GlRenderer *renderer)
    {
        int linespace = renderer->GetLinespace();

        // TODO: Don't need to calculate the cursor pos each time, only when changes
        int cursorX, cursorWidth, cursorHeight;
        renderer->CalculateBlockCursorPos(inputLine_.GetCursorPos(), &cursorX, &cursorWidth, &cursorHeight);

        int cursorY = linespace * lines_.size();

        int y = 0;

        // Console background
        renderer->Begin2D();
        renderer->Color(0, 0, 0, 0.7f);
        renderer->Rect(0, 0, renderer->Width(), renderer->Height());

        // Block cursor background
        renderer->DrawBlockCursorBackground(cursorX, cursorY, cursorWidth, cursorHeight);

        // Draw text
        renderer->BeginText();
        for (auto line : lines_) {
            renderer->Text(0, y, line);
            y += linespace;
        }
        renderer->Text(0, y, inputLine_.GetLine());

        // Do block cursor inversion
        renderer->Begin2D();
        renderer->DrawBlockCursorInversion(cursorX, cursorY, cursorWidth, cursorHeight);

        renderer->Begin3D();
    }

    void Console::InputText(char *text)
    {
        inputLine_.Insert(text[0]);
    }

    void Console::Println(const std::string &str)
    {
        lines_.push_back(str);
    }

    void Console::ProcessKey(SDL_KeyboardEvent *key)
    {
        switch (key->keysym.sym) {
        case SDLK_BACKSPACE:
            inputLine_.DeleteBack();
            break;
        case SDLK_LEFT:
            inputLine_.BackwardChar();
            break;
        case SDLK_RIGHT:
            inputLine_.ForwardChar();
            break;
        case SDLK_RETURN:
            lines_.push_back(inputLine_.GetLine());
            ProcessReturn();
            inputLine_.Clear(continuedLine_ ? continuedLinePrompt_ : topLevelPrompt_);
            break;
        }
    }

    void Console::ProcessReturn()
    {
        cmd_ += "\n";
        cmd_ += inputLine_.GetUserText();
        switch (cmdProcessor_->CheckCommand(cmd_)) {
        case BalancedState::Balanced:
            lines_.push_back(cmdProcessor_->DoCommand(cmd_));
            cmd_ = "";
            continuedLine_ = false;
            break;
        case BalancedState::TooManyClosingParens:
            lines_.push_back("Too many closing parens");
            cmd_ = "";
            continuedLine_ = false;
            break;
        case BalancedState::NotClosed:
            continuedLine_ = true;
            break;
        }
    }

}
