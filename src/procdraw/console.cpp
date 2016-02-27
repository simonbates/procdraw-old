#include "console.h"

namespace procdraw {

    void Console::Draw(GlRenderer *renderer)
    {
        // TODO: Don't need to calculate the cursor pos each time, only when changes
        // TODO: Calculate y positioning from font metrics
        int cursorX, cursorWidth;
        int cursorY = 47;
        int cursorHeight = 42;
        renderer->CalculateBlockCursorPos(line_.GetCursorPos(), &cursorX, &cursorWidth);

        // Console background
        renderer->Begin2D();
        renderer->Color(0, 0, 1.0f, 0.5f);
        renderer->Rect(0, 0, renderer->Width(), renderer->Height());

        // Block cursor background
        renderer->Color(0, 0, 1.0f, 1.0f);
        renderer->Rect(cursorX, cursorY, cursorWidth, cursorHeight);

        // Draw text
        renderer->BeginText();
        renderer->Text(0, 0, line_.GetLine());

        // Do block cursor inversion
        renderer->Begin2D();
        renderer->BeginInverse();
        renderer->Rect(cursorX, cursorY, cursorWidth, cursorHeight);
        renderer->EndInverse();

        renderer->Begin3D();
    }

    void Console::InputText(char *text)
    {
        line_.Insert(text[0]);
    }

    void Console::ProcessKey(SDL_KeyboardEvent *key)
    {
        switch (key->keysym.sym) {
        case SDLK_BACKSPACE:
            line_.DeleteBack();
            break;
        case SDLK_LEFT:
            line_.BackwardChar();
            break;
        case SDLK_RIGHT:
            line_.ForwardChar();
            break;
        }
    }

}
