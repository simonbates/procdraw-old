#include "util.h"
#include "catch.hpp"

TEST_CASE("Clamp")
{

    SECTION("<int>") {
        SECTION("should return the lower bound for a value below the range") {
            REQUIRE(procdraw::Clamp(50, 100, 200) == 100);
        }
        SECTION("should return the value as is for a value within the range") {
            REQUIRE(procdraw::Clamp(150, 100, 200) == 150);
        }
        SECTION("should return the upper bound for a value above the range") {
            REQUIRE(procdraw::Clamp(250, 100, 200) == 200);
        }
    }

    SECTION("<float>") {
        SECTION("should return the lower bound for a value below the range") {
            REQUIRE(procdraw::Clamp(50.0f, 100.0f, 200.0f) == 100.0f);
        }
        SECTION("should return the value as is for a value within the range") {
            REQUIRE(procdraw::Clamp(150.0f, 100.0f, 200.0f) == 150.0f);
        }
        SECTION("should return the upper bound for a value above the range") {
            REQUIRE(procdraw::Clamp(250.0f, 100.0f, 200.0f) == 200.0f);
        }
    }

}

TEST_CASE("Lerp")
{

    SECTION("should interpolate values for [0, 8]") {
        REQUIRE(procdraw::Lerp(0.0, 8.0, 0.0) == 0.0);
        REQUIRE(procdraw::Lerp(0.0, 8.0, 0.25) == 2.0);
        REQUIRE(procdraw::Lerp(0.0, 8.0, 0.5) == 4.0);
        REQUIRE(procdraw::Lerp(0.0, 8.0, 0.75) == 6.0);
        REQUIRE(procdraw::Lerp(0.0, 8.0, 1.0) == 8.0);
    }

    SECTION("should interpolate values for [4, -4]") {
        REQUIRE(procdraw::Lerp(4.0, -4.0, 0.0) == 4.0);
        REQUIRE(procdraw::Lerp(4.0, -4.0, 0.25) == 2.0);
        REQUIRE(procdraw::Lerp(4.0, -4.0, 0.5) == 0.0);
        REQUIRE(procdraw::Lerp(4.0, -4.0, 0.75) == -2.0);
        REQUIRE(procdraw::Lerp(4.0, -4.0, 1.0) == -4.0);
    }

}

TEST_CASE("MapRange")
{

    SECTION("should map values from [0, 10] to [-1, 0]") {
        REQUIRE(procdraw::MapRange(0.0, 10.0, -1.0, 0.0, 0.0) == -1.0);
        REQUIRE(procdraw::MapRange(0.0, 10.0, -1.0, 0.0, 2.5) == -0.75);
        REQUIRE(procdraw::MapRange(0.0, 10.0, -1.0, 0.0, 5.0) == -0.5);
        REQUIRE(procdraw::MapRange(0.0, 10.0, -1.0, 0.0, 7.5) == -0.25);
        REQUIRE(procdraw::MapRange(0.0, 10.0, -1.0, 0.0, 10.0) == 0.0);
    }

    SECTION("should map values from [0, 10] to [1, -1]") {
        REQUIRE(procdraw::MapRange(0.0, 10.0, 1.0, -1.0, 0.0) == 1.0);
        REQUIRE(procdraw::MapRange(0.0, 10.0, 1.0, -1.0, 2.5) == 0.5);
        REQUIRE(procdraw::MapRange(0.0, 10.0, 1.0, -1.0, 5.0) == 0.0);
        REQUIRE(procdraw::MapRange(0.0, 10.0, 1.0, -1.0, 7.5) == -0.5);
        REQUIRE(procdraw::MapRange(0.0, 10.0, 1.0, -1.0, 10.0) == -1.0);
    }

}

TEST_CASE("Norm")
{

    SECTION("should normalize values for [0, 8]") {
        REQUIRE(procdraw::Norm(0.0, 8.0, 0.0) == 0.0);
        REQUIRE(procdraw::Norm(0.0, 8.0, 2.0) == 0.25);
        REQUIRE(procdraw::Norm(0.0, 8.0, 4.0) == 0.5);
        REQUIRE(procdraw::Norm(0.0, 8.0, 6.0) == 0.75);
        REQUIRE(procdraw::Norm(0.0, 8.0, 8.0) == 1.0);
    }

    SECTION("should normalize values for [4, -4]") {
        REQUIRE(procdraw::Norm(4.0, -4.0, 4.0) == 0.0);
        REQUIRE(procdraw::Norm(4.0, -4.0, 2.0) == 0.25);
        REQUIRE(procdraw::Norm(4.0, -4.0, 0.0) == 0.5);
        REQUIRE(procdraw::Norm(4.0, -4.0, -2.0) == 0.75);
        REQUIRE(procdraw::Norm(4.0, -4.0, -4.0) == 1.0);
    }

}

TEST_CASE("Wrap")
{

    SECTION("should wrap values for [0, 10]") {
        REQUIRE(procdraw::Wrap(0.0, 10.0, 0.0) == 0.0);
        REQUIRE(procdraw::Wrap(0.0, 10.0, 10.0) == 0.0);
        REQUIRE(procdraw::Wrap(0.0, 10.0, 20.0) == 0.0);
        REQUIRE(procdraw::Wrap(0.0, 10.0, -10.0) == 0.0);
        REQUIRE(procdraw::Wrap(0.0, 10.0, 8.0) == 8.0);
        REQUIRE(procdraw::Wrap(0.0, 10.0, 12.0) == 2.0);
        REQUIRE(procdraw::Wrap(0.0, 10.0, 23.0) == 3.0);
        REQUIRE(procdraw::Wrap(0.0, 10.0, -2.0) == 8.0);
        REQUIRE(procdraw::Wrap(0.0, 10.0, -13.0) == 7.0);
    }

    SECTION("should wrap values for [-20, -10]") {
        REQUIRE(procdraw::Wrap(-20.0, -10.0, -20.0) == -20.0);
        REQUIRE(procdraw::Wrap(-20.0, -10.0, -10.0) == -20.0);
        REQUIRE(procdraw::Wrap(-20.0, -10.0, 0.0) == -20.0);
        REQUIRE(procdraw::Wrap(-20.0, -10.0, -30.0) == -20.0);
        REQUIRE(procdraw::Wrap(-20.0, -10.0, -12.0) == -12.0);
        REQUIRE(procdraw::Wrap(-20.0, -10.0, -8.0) == -18.0);
        REQUIRE(procdraw::Wrap(-20.0, -10.0, 13.0) == -17.0);
        REQUIRE(procdraw::Wrap(-20.0, -10.0, -22.0) == -12.0);
        REQUIRE(procdraw::Wrap(-20.0, -10.0, -33.0) == -13.0);
    }

}

TEST_CASE("SimpleMovingAverage")
{

    procdraw::SimpleMovingAverage<int> sma(4);

    SECTION("should calculate the average for a moving window") {
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

}
