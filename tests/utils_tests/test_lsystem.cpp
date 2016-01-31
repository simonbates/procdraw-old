#include "lsystem.h"
#include "catch.hpp"

TEST_CASE("Lsystem Algae")
{
    // Algae -- variables only

    procdraw::Lsystem algae("A", { { 'A', "AB" }, { 'B', "A" } });

    REQUIRE(algae.GetString() == std::string("A"));
    algae.Next();
    REQUIRE(algae.GetString() == std::string("AB"));
    algae.Next();
    REQUIRE(algae.GetString() == std::string("ABA"));
    algae.Next();
    REQUIRE(algae.GetString() == std::string("ABAAB"));
    algae.Next();
    REQUIRE(algae.GetString() == std::string("ABAABABA"));
}

TEST_CASE("Lsystem Koch Curve")
{
    // Koch Curve -- variables and constants

    procdraw::Lsystem kochCurve("F", { { 'F', "F-F++F-F" } });

    REQUIRE(kochCurve.GetString() == std::string("F"));
    kochCurve.Next();
    REQUIRE(kochCurve.GetString() == std::string("F-F++F-F"));
    kochCurve.Next();
    REQUIRE(kochCurve.GetString() == std::string("F-F++F-F-F-F++F-F++F-F++F-F-F-F++F-F"));
}
