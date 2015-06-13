#define CATCH_CONFIG_MAIN

#include "color.h"
#include "catch.hpp"

TEST_CASE("Hsv2Rgb red")
{
    float r, g, b;
    procdraw::Hsv2Rgb(0.0f, 1.0f, 1.0f, r, g, b);
    REQUIRE(r == 1.0f);
    REQUIRE(g == 0.0f);
    REQUIRE(b == 0.0f);
}

TEST_CASE("Hsv2Rgb green")
{
    float r, g, b;
    procdraw::Hsv2Rgb(120.0f, 1.0f, 1.0f, r, g, b);
    REQUIRE(r == 0.0f);
    REQUIRE(g == 1.0f);
    REQUIRE(b == 0.0f);
}

TEST_CASE("Hsv2Rgb blue")
{
    float r, g, b;
    procdraw::Hsv2Rgb(240.0f, 1.0f, 1.0f, r, g, b);
    REQUIRE(r == 0.0f);
    REQUIRE(g == 0.0f);
    REQUIRE(b == 1.0f);
}
