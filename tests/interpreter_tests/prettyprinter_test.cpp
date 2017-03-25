#include "procdraw/interpreter/prettyprinter.h"
#include "procdraw/interpreter/lisp_interpreter.h"
#include "gtest/gtest.h"
#include <string>

class PrettyPrinterTest : public ::testing::Test {
protected:
    virtual void SetUp()
    {
        listOfNumbers_ = L_.Read("(10 20 30 40 50 60 70 80)");
        listOfSymbols_ = L_.Read("(aaa bbb ccc ddd)");
        listOfLists_ = L_.MakeList({ listOfNumbers_, listOfSymbols_ });
        lambdaNoParams_ = L_.Read("(lambda () (call1) (call2 arg1 arg2 arg3))");
        lambdaWithParams_ = L_.Read(
            "(lambda (param1 param2) (call1) (call2 arg1 arg2 arg3))");
        lambdaLongParamList_ = L_.Read("(lambda (param1 param2 param3 param4) "
                                       "(call1) (call2 arg1 arg2 arg3))");
    }

    procdraw::PrettyPrinter prettyPrinter_;
    procdraw::LispInterpreter L_;
    procdraw::LispObjectPtr listOfNumbers_;
    procdraw::LispObjectPtr listOfSymbols_;
    procdraw::LispObjectPtr listOfLists_;
    procdraw::LispObjectPtr lambdaNoParams_;
    procdraw::LispObjectPtr lambdaWithParams_;
    procdraw::LispObjectPtr lambdaLongParamList_;
};

TEST_F(PrettyPrinterTest, PrintAtoms)
{
    EXPECT_EQ("42", prettyPrinter_.PrintToString(&L_, L_.MakeNumber(42), 100));
    EXPECT_EQ(
        "-42", prettyPrinter_.PrintToString(&L_, L_.MakeNumber(-42), 100));
    EXPECT_EQ("true", prettyPrinter_.PrintToString(&L_, L_.True, 100));
    EXPECT_EQ("false", prettyPrinter_.PrintToString(&L_, L_.False, 100));
    EXPECT_EQ("\"some string\"",
        prettyPrinter_.PrintToString(&L_, L_.MakeString("some string"), 100));
    EXPECT_EQ(
        "HELLO", prettyPrinter_.PrintToString(&L_, L_.SymbolRef("HELLO"), 100));
}

TEST_F(PrettyPrinterTest, PrintEmptyListAsNil)
{
    EXPECT_EQ("nil", prettyPrinter_.PrintToString(&L_, L_.MakeList({}), 100));
}

TEST_F(PrettyPrinterTest, PrintListOfNumbersAllOneLine)
{
    std::string expected = "(10 20 30 40 50 60 70 80)";
    EXPECT_EQ(expected, prettyPrinter_.PrintToString(&L_, listOfNumbers_, 25));
}

TEST_F(PrettyPrinterTest, PrintListOfNumbersWithBreaks)
{
    std::string expected = "(10\n"
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
    std::string expected = "((10 20 30 40 50 60 70 80) (aaa bbb ccc ddd))";
    EXPECT_EQ(expected, prettyPrinter_.PrintToString(&L_, listOfLists_, 45));
}

TEST_F(PrettyPrinterTest, PrintListOfListsOnTwoLines)
{
    std::string expected = "((10 20 30 40 50 60 70 80)\n"
                           " (aaa bbb ccc ddd))";
    EXPECT_EQ(expected, prettyPrinter_.PrintToString(&L_, listOfLists_, 44));
    EXPECT_EQ(expected, prettyPrinter_.PrintToString(&L_, listOfLists_, 26));
}

TEST_F(PrettyPrinterTest, PrintListOfListsFitSecond)
{
    std::string expected = "((10\n"
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
    std::string expected = "((10\n"
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

TEST_F(PrettyPrinterTest, PrintLambdaAllOneLine)
{
    EXPECT_EQ("(lambda nil (call1) (call2 arg1 arg2 arg3))",
        prettyPrinter_.PrintToString(&L_, lambdaNoParams_, 100));
    EXPECT_EQ("(lambda (param1 param2) (call1) (call2 arg1 arg2 arg3))",
        prettyPrinter_.PrintToString(&L_, lambdaWithParams_, 100));
    EXPECT_EQ(
        "(lambda (param1 param2 param3 param4) (call1) (call2 arg1 arg2 arg3))",
        prettyPrinter_.PrintToString(&L_, lambdaLongParamList_, 100));
}

TEST_F(PrettyPrinterTest, PrintLambdaFitParamsAndBreaksBetweenBodyForms)
{
    std::string expectedNoParams = "(lambda nil\n"
                                   "  (call1)\n"
                                   "  (call2 arg1 arg2 arg3))";

    EXPECT_EQ(expectedNoParams,
        prettyPrinter_.PrintToString(&L_, lambdaNoParams_, 25));

    std::string expectedWithParams = "(lambda (param1 param2)\n"
                                     "  (call1)\n"
                                     "  (call2 arg1 arg2 arg3))";

    EXPECT_EQ(expectedWithParams,
        prettyPrinter_.PrintToString(&L_, lambdaWithParams_, 25));

    std::string expectedLongParamList
        = "(lambda (param1 param2 param3 param4)\n"
          "  (call1)\n"
          "  (call2 arg1 arg2 arg3))";

    EXPECT_EQ(expectedLongParamList,
        prettyPrinter_.PrintToString(&L_, lambdaLongParamList_, 37));
}

TEST_F(PrettyPrinterTest, PrintLambdaBreakLongParamList)
{
    std::string expected = "(lambda (param1\n"
                           "         param2\n"
                           "         param3\n"
                           "         param4)\n"
                           "  (call1)\n"
                           "  (call2 arg1 arg2 arg3))";

    EXPECT_EQ(
        expected, prettyPrinter_.PrintToString(&L_, lambdaLongParamList_, 36));
}

TEST_F(PrettyPrinterTest, PrintLambdaBreaksEverywhere)
{
    std::string expected = "(lambda (param1\n"
                           "         param2\n"
                           "         param3\n"
                           "         param4)\n"
                           "  (call1)\n"
                           "  (call2\n"
                           "   arg1\n"
                           "   arg2\n"
                           "   arg3))";

    EXPECT_EQ(
        expected, prettyPrinter_.PrintToString(&L_, lambdaLongParamList_, 24));
}

TEST_F(PrettyPrinterTest, PrintIncompleteLambda)
{
    EXPECT_EQ("(lambda)",
        prettyPrinter_.PrintToString(&L_, L_.Read("(lambda)"), 100));
    EXPECT_EQ("(lambda nil)",
        prettyPrinter_.PrintToString(&L_, L_.Read("(lambda ())"), 100));
    EXPECT_EQ("(lambda nil nil)",
        prettyPrinter_.PrintToString(&L_, L_.Read("(lambda () nil)"), 100));
}

TEST_F(PrettyPrinterTest, PrintDottedPairs)
{
    EXPECT_EQ("(1 . 2)", prettyPrinter_.PrintToString(&L_,
                             L_.Cons(L_.MakeNumber(1), L_.MakeNumber(2)), 100));
    EXPECT_EQ("(2 3 . 4)", prettyPrinter_.PrintToString(&L_,
                               L_.Cons(L_.MakeNumber(2),
                                   L_.Cons(L_.MakeNumber(3), L_.MakeNumber(4))),
                               100));
}

TEST_F(PrettyPrinterTest, PrintQuoteForm)
{
    EXPECT_EQ("'42",
        prettyPrinter_.PrintToString(&L_,
            L_.MakeList({ L_.SymbolRef("quote"), L_.MakeNumber(42) }), 100));
}

TEST_F(PrettyPrinterTest, PrintSigvalForm)
{
    EXPECT_EQ("$foo",
        prettyPrinter_.PrintToString(&L_,
            L_.MakeList({ L_.SymbolRef("sigval"), L_.SymbolRef("foo") }), 100));
}

TEST_F(PrettyPrinterTest, PrintDictionaries)
{
    EXPECT_EQ(
        "<Dictionary>", prettyPrinter_.PrintToString(&L_, L_.MakeDict(), 100));
}
