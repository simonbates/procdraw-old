#include "util.h"
#include "catch.hpp"

TEST_CASE("Clamp int") {
    REQUIRE(procdraw::Clamp(50, 100, 200) == 100);
    REQUIRE(procdraw::Clamp(150, 100, 200) == 150);
    REQUIRE(procdraw::Clamp(250, 100, 200) == 200);
}

TEST_CASE("Clamp float") {
    REQUIRE(procdraw::Clamp(50.0f, 100.0f, 200.0f) == 100.0f);
    REQUIRE(procdraw::Clamp(150.0f, 100.0f, 200.0f) == 150.0f);
    REQUIRE(procdraw::Clamp(250.0f, 100.0f, 200.0f) == 200.0f);
}
