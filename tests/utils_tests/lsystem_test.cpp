#include "procdraw/utils/lsystem.h"
#include "gtest/gtest.h"

TEST(LsystemTest, Algae)
{
    // Algae -- variables only

    procdraw::Lsystem algae("A", { { 'A', "AB" }, { 'B', "A" } });

    EXPECT_EQ(std::string("A"), algae.GetString());
    algae.Next();
    EXPECT_EQ(std::string("AB"), algae.GetString());
    algae.Next();
    EXPECT_EQ(std::string("ABA"), algae.GetString());
    algae.Next();
    EXPECT_EQ(std::string("ABAAB"), algae.GetString());
    algae.Next();
    EXPECT_EQ(std::string("ABAABABA"), algae.GetString());
}

TEST(LsystemTest, KochCurve)
{
    // Koch Curve -- variables and constants

    procdraw::Lsystem kochCurve("F", { { 'F', "F-F++F-F" } });

    EXPECT_EQ(std::string("F"), kochCurve.GetString());
    kochCurve.Next();
    EXPECT_EQ(std::string("F-F++F-F"), kochCurve.GetString());
    kochCurve.Next();
    EXPECT_EQ(std::string("F-F++F-F-F-F++F-F++F-F++F-F-F-F++F-F"), kochCurve.GetString());
}
