#include "lsystem.h"
#include "catch.hpp"

TEST_CASE("Lsystem Algae") {
    // Algae -- variables only

    // TODO when upgrade to VS2013, switch to list initialization
    std::map<char, std::string> rules;
    rules['A'] = "AB";
    rules['B'] = "A";
    procdraw::Lsystem algae("A", rules);

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

TEST_CASE("Lsystem Koch Curve") {
    // Koch Curve -- variables and constants

    // TODO when upgrade to VS2013, switch to list initialization
    std::map<char, std::string> rules;
    rules['F'] = "F-F++F-F";
    procdraw::Lsystem kochCurve("F", rules);

    REQUIRE(kochCurve.GetString() == std::string("F"));
    kochCurve.Next();
    REQUIRE(kochCurve.GetString() == std::string("F-F++F-F"));
    kochCurve.Next();
    REQUIRE(kochCurve.GetString() == std::string("F-F++F-F-F-F++F-F++F-F++F-F-F-F++F-F"));
}
