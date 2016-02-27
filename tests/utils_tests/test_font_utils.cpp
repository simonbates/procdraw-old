#include "font_utils.h"
#include "catch.hpp"

TEST_CASE("CalculateFixedWidthBlockCursorPos")
{
    int x, width;

    procdraw::CalculateFixedWidthBlockCursorPos(0, 10, &x, &width);
    REQUIRE(x == 0);
    REQUIRE(width == 10);

    procdraw::CalculateFixedWidthBlockCursorPos(2, 10, &x, &width);
    REQUIRE(x == 20);
    REQUIRE(width == 10);
}
