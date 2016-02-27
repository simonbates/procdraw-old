#include "console.h"

namespace procdraw {

    void Console::Draw(GlRenderer *renderer)
    {
        // TODO: Don't need to calculate the cursor pos each time, only when changes
        int cursorX, cursorWidth, cursorHeight;
        renderer->CalculateBlockCursorPos(line_.GetCursorPos(), &cursorX, &cursorWidth, &cursorHeight);

        // Console background
        renderer->Begin2D();
        renderer->Color(0, 0, 1.0f, 0.5f);
        renderer->Rect(0, 0, renderer->Width(), renderer->Height());

        // Block cursor background
        renderer->DrawBlockCursorBackground(cursorX, 0, cursorWidth, cursorHeight);

        // Draw text
        renderer->BeginText();
        renderer->Text(0, 0, line_.GetLine());

        // Do block cursor inversion
        renderer->Begin2D();
        renderer->DrawBlockCursorInversion(cursorX, 0, cursorWidth, cursorHeight);

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
