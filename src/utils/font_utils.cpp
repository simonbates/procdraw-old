#include "font_utils.h"

namespace procdraw {

    void CalculateFixedWidthBlockCursorPos(int cursorTextPosition, int glyphWidth, int *x, int *width)
    {
        *x = cursorTextPosition * glyphWidth;
        *width = glyphWidth;
    }

}
