#include "procdraw/interpreter/prettyprinter.h"
#include "procdraw/interpreter/lisp_interpreter.h"
#include "gtest/gtest.h"

class PrettyPrinterTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        listOfNumbers_ = L_.MakeList({
            L_.MakeNumber(10),
            L_.MakeNumber(20),
            L_.MakeNumber(30),
            L_.MakeNumber(40),
            L_.MakeNumber(50),
            L_.MakeNumber(60),
            L_.MakeNumber(70),
            L_.MakeNumber(80)
        });

        listOfSymbols_ = L_.MakeList({
            L_.SymbolRef("aaa"),
            L_.SymbolRef("bbb"),
            L_.SymbolRef("ccc"),
            L_.SymbolRef("ddd")
        });
    }

    procdraw::PrettyPrinter prettyPrinter_;
    procdraw::LispInterpreter L_;
    procdraw::LispObjectPtr listOfNumbers_;
    procdraw::LispObjectPtr listOfSymbols_;
};

TEST_F(PrettyPrinterTest, PrintAtoms)
{
    EXPECT_EQ("42", prettyPrinter_.PrintToString(&L_, L_.MakeNumber(42), 20, 2));

    // TODO: Test other atoms
}

TEST_F(PrettyPrinterTest, PrintListOfNumbersAllOneLine)
{
    std::string expected = "(10 20 30 40 50 60 70 80)";
    EXPECT_EQ(expected, prettyPrinter_.PrintToString(&L_, listOfNumbers_, 25, 2));
}

TEST_F(PrettyPrinterTest, PrintListOfNumbersWithBreak)
{
    std::string expected =
        "(10 20 30 40 50 60 70\n"
        "  80)";
    EXPECT_EQ(expected, prettyPrinter_.PrintToString(&L_, listOfNumbers_, 24, 2));
}

TEST_F(PrettyPrinterTest, PrintListOfNumbersWithTwoBreaks)
{
    std::string expected =
        "(10 20 30\n"
        "  40 50 60\n"
        "  70 80)";
    EXPECT_EQ(expected, prettyPrinter_.PrintToString(&L_, listOfNumbers_, 10, 2));
}

TEST_F(PrettyPrinterTest, PrintListOfLists)
{
    auto obj = L_.MakeList({ listOfNumbers_, listOfSymbols_ });
    std::string expected =
        "((10 20 30 40 50 60 70 80)\n"
        "  (aaa bbb ccc ddd))";
    EXPECT_EQ(expected, prettyPrinter_.PrintToString(&L_, obj, 40, 2));
}
