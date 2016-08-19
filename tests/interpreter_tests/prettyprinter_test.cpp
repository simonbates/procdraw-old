#include "procdraw/interpreter/prettyprinter.h"
#include "procdraw/interpreter/lisp_interpreter.h"
#include "gtest/gtest.h"

class PrettyPrinterTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        listOfNumbers_ = L_.Read("(10 20 30 40 50 60 70 80)");
        listOfSymbols_ = L_.Read("(aaa bbb ccc ddd)");
        listOfLists_ = L_.MakeList({ listOfNumbers_, listOfSymbols_ });
    }

    procdraw::PrettyPrinter prettyPrinter_;
    procdraw::LispInterpreter L_;
    procdraw::LispObjectPtr listOfNumbers_;
    procdraw::LispObjectPtr listOfSymbols_;
    procdraw::LispObjectPtr listOfLists_;
};

TEST_F(PrettyPrinterTest, PrintAtoms)
{
    EXPECT_EQ("42", prettyPrinter_.PrintToString(&L_, L_.MakeNumber(42), 20));

    // TODO: Test other atoms
}

TEST_F(PrettyPrinterTest, PrintListOfNumbersAllOneLine)
{
    std::string expected = "(10 20 30 40 50 60 70 80)";
    EXPECT_EQ(expected, prettyPrinter_.PrintToString(&L_, listOfNumbers_, 25));
}

TEST_F(PrettyPrinterTest, PrintListOfNumbersWithBreaks)
{
    std::string expected =
        "(10\n"
        " 20\n"
        " 30\n"
        " 40\n"
        " 50\n"
        " 60\n"
        " 70\n"
        " 80)";
    EXPECT_EQ(expected, prettyPrinter_.PrintToString(&L_, listOfNumbers_, 24));
}

TEST_F(PrettyPrinterTest, PrintListOfListsAllOneLine)
{
    std::string expected =
        "((10 20 30 40 50 60 70 80) (aaa bbb ccc ddd))";
    EXPECT_EQ(expected, prettyPrinter_.PrintToString(&L_, listOfLists_, 45));
}

TEST_F(PrettyPrinterTest, PrintListOfListsOnTwoLines)
{
    std::string expected =
        "((10 20 30 40 50 60 70 80)\n"
        " (aaa bbb ccc ddd))";
    EXPECT_EQ(expected, prettyPrinter_.PrintToString(&L_, listOfLists_, 44));
    EXPECT_EQ(expected, prettyPrinter_.PrintToString(&L_, listOfLists_, 26));
}

TEST_F(PrettyPrinterTest, PrintListOfListsFitSecond)
{
    std::string expected =
        "((10\n"
        "  20\n"
        "  30\n"
        "  40\n"
        "  50\n"
        "  60\n"
        "  70\n"
        "  80)\n"
        " (aaa bbb ccc ddd))";
    EXPECT_EQ(expected, prettyPrinter_.PrintToString(&L_, listOfLists_, 25));
    EXPECT_EQ(expected, prettyPrinter_.PrintToString(&L_, listOfLists_, 19));
}

TEST_F(PrettyPrinterTest, PrintListOfListsAllBreaks)
{
    std::string expected =
        "((10\n"
        "  20\n"
        "  30\n"
        "  40\n"
        "  50\n"
        "  60\n"
        "  70\n"
        "  80)\n"
        " (aaa\n"
        "  bbb\n"
        "  ccc\n"
        "  ddd))";
    EXPECT_EQ(expected, prettyPrinter_.PrintToString(&L_, listOfLists_, 18));
}
