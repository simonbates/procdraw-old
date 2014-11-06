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

TEST_CASE("SimpleMovingAverage") {
    procdraw::SimpleMovingAverage<int> sma(4);

    REQUIRE(sma.GetMean() == 0);

    sma.AddDataPoint(8);
    // 0, 0, 0, 8
    REQUIRE(sma.GetMean() == 2);

    sma.AddDataPoint(16);
    // 0, 0, 8, 16
    REQUIRE(sma.GetMean() == 6);

    sma.AddDataPoint(32);
    // 0, 8, 16, 32
    REQUIRE(sma.GetMean() == 14);

    sma.AddDataPoint(64);
    // 8, 16, 32, 64
    REQUIRE(sma.GetMean() == 30);

    sma.AddDataPoint(128);
    // 16, 32, 64, 128
    REQUIRE(sma.GetMean() == 60);

    sma.AddDataPoint(0);
    // 32, 64, 128, 0
    REQUIRE(sma.GetMean() == 56);
}
