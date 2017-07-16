#include "procdraw/interpreter/lisp_reader.h"
#include "procdraw/interpreter/lisp_interpreter.h"
#include <gtest/gtest.h>

class LispReaderTest : public ::testing::Test {
protected:
    procdraw::LispReader reader_;
    procdraw::LispInterpreter L_;
};

TEST_F(LispReaderTest, ReadIntegerWithoutSign)
{
    auto obj = reader_.Read(&L_, "42");
    EXPECT_EQ(procdraw::LispObjectType::Number, L_.TypeOf(obj));
    EXPECT_EQ(42, L_.NumVal(obj));
}

TEST_F(LispReaderTest, ReadIntegerWithPositiveSign)
{
    auto obj = reader_.Read(&L_, "+42");
    EXPECT_EQ(procdraw::LispObjectType::Number, L_.TypeOf(obj));
    EXPECT_EQ(42, L_.NumVal(obj));
}

TEST_F(LispReaderTest, ReadIntegerWithNegativeSign)
{
    auto obj = reader_.Read(&L_, "-42");
    EXPECT_EQ(procdraw::LispObjectType::Number, L_.TypeOf(obj));
    EXPECT_EQ(-42, L_.NumVal(obj));
}

TEST_F(LispReaderTest, ReadSymbol)
{
    auto obj = reader_.Read(&L_, "HELLO-WORLD-1");
    EXPECT_EQ(procdraw::LispObjectType::Symbol, L_.TypeOf(obj));
    EXPECT_EQ("HELLO-WORLD-1", L_.SymbolName(obj));
}

TEST_F(LispReaderTest, ReadDollarAsSigval)
{
    auto obj = reader_.Read(&L_, "$foo");
    EXPECT_EQ(procdraw::LispObjectType::Cons, L_.TypeOf(obj));
    EXPECT_EQ("sigval", L_.SymbolName(L_.Car(obj)));
    EXPECT_EQ("foo", L_.SymbolName(L_.Cadr(obj)));
    EXPECT_TRUE(L_.Null(L_.Cddr(obj)));
}

TEST_F(LispReaderTest, ReadQuote)
{
    auto obj = reader_.Read(&L_, "'42");
    EXPECT_EQ(procdraw::LispObjectType::Cons, L_.TypeOf(obj));
    EXPECT_EQ("quote", L_.SymbolName(L_.Car(obj)));
    EXPECT_EQ(42, L_.NumVal(L_.Cadr(obj)));
    EXPECT_TRUE(L_.Null(L_.Cddr(obj)));
}

TEST_F(LispReaderTest, ReadStarCharAsSymbol)
{
    auto obj = reader_.Read(&L_, "*");
    EXPECT_EQ(procdraw::LispObjectType::Symbol, L_.TypeOf(obj));
    EXPECT_EQ("*", L_.SymbolName(obj));
}

TEST_F(LispReaderTest, ReadPlusCharAsSymbol)
{
    auto obj = reader_.Read(&L_, "(+ 42)");
    EXPECT_EQ(procdraw::LispObjectType::Symbol, L_.TypeOf(L_.Car(obj)));
    EXPECT_EQ("+", L_.SymbolName(L_.Car(obj)));
    EXPECT_EQ(procdraw::LispObjectType::Number, L_.TypeOf(L_.Cadr(obj)));
    EXPECT_EQ(42, L_.NumVal(L_.Cadr(obj)));
}

TEST_F(LispReaderTest, ReadHyphenMinusCharAsSymbol)
{
    auto obj = reader_.Read(&L_, "(- 42)");
    EXPECT_EQ(procdraw::LispObjectType::Symbol, L_.TypeOf(L_.Car(obj)));
    EXPECT_EQ("-", L_.SymbolName(L_.Car(obj)));
    EXPECT_EQ(procdraw::LispObjectType::Number, L_.TypeOf(L_.Cadr(obj)));
    EXPECT_EQ(42, L_.NumVal(L_.Cadr(obj)));
}

TEST_F(LispReaderTest, ReadSlashCharAsSymbol)
{
    auto obj = reader_.Read(&L_, "/");
    EXPECT_EQ(procdraw::LispObjectType::Symbol, L_.TypeOf(obj));
    EXPECT_EQ("/", L_.SymbolName(obj));
}

TEST_F(LispReaderTest, ReadArrowAsSymbol)
{
    auto obj = reader_.Read(&L_, "(=>42)");
    EXPECT_EQ(procdraw::LispObjectType::Symbol, L_.TypeOf(L_.Car(obj)));
    EXPECT_EQ("=>", L_.SymbolName(L_.Car(obj)));
    EXPECT_EQ(42, L_.NumVal(L_.Cadr(obj)));
}

TEST_F(LispReaderTest, ReadNilKeyword)
{
    auto obj = reader_.Read(&L_, "nil");
    EXPECT_TRUE(L_.Null(obj));
}

TEST_F(LispReaderTest, ReadEmptyListAsNil)
{
    auto obj = reader_.Read(&L_, "()");
    EXPECT_TRUE(L_.Null(obj));
}

TEST_F(LispReaderTest, ReadListWithSingleItem)
{
    auto obj = reader_.Read(&L_, "(42)");
    EXPECT_EQ(procdraw::LispObjectType::Cons, L_.TypeOf(obj));
    EXPECT_EQ(42, L_.NumVal(L_.Car(obj)));
    EXPECT_TRUE(L_.Null(L_.Cdr(obj)));
}

// +---+  +---+  +---+  +---+
// |1|*|->|2|*|->|3|*|->|4|/|
// +---+  +---+  +---+  +---+
//
TEST_F(LispReaderTest, ReadListWithMultipleItems)
{
    auto list1 = reader_.Read(&L_, "(1 2 3 4)");
    EXPECT_EQ(procdraw::LispObjectType::Cons, L_.TypeOf(list1));
    EXPECT_EQ(1, L_.NumVal(L_.Car(list1)));
    EXPECT_EQ(2, L_.NumVal(L_.Car(L_.Cdr(list1))));
    EXPECT_EQ(3, L_.NumVal(L_.Car(L_.Cdr(L_.Cdr(list1)))));
    EXPECT_EQ(4, L_.NumVal(L_.Car(L_.Cdr(L_.Cdr(L_.Cdr(list1))))));
    EXPECT_TRUE(L_.Null(L_.Cdr(L_.Cdr(L_.Cdr(L_.Cdr(list1))))));
}

// +---+  +---+  +---+
// |*|*|->|3|*|->|4|/|
// +---+  +---+  +---+
//  |
//  v
// +---+  +---+
// |1|*|->|2|/|
// +---+  +---+
//
TEST_F(LispReaderTest, ReadListWithEmbeddedList)
{
    auto list1 = reader_.Read(&L_, "((1 2) 3 4)");
    EXPECT_EQ(procdraw::LispObjectType::Cons, L_.TypeOf(list1));
    EXPECT_EQ(1, L_.NumVal(L_.Car(L_.Car(list1))));
    EXPECT_EQ(2, L_.NumVal(L_.Car(L_.Cdr(L_.Car(list1)))));
    EXPECT_TRUE(L_.Null(L_.Cdr(L_.Cdr(L_.Car(list1)))));
    EXPECT_EQ(3, L_.NumVal(L_.Car(L_.Cdr(list1))));
    EXPECT_EQ(4, L_.NumVal(L_.Car(L_.Cdr(L_.Cdr(list1)))));
    EXPECT_TRUE(L_.Null(L_.Cdr(L_.Cdr(L_.Cdr(list1)))));
}

TEST_F(LispReaderTest, ReadDottedPair)
{
    auto obj = reader_.Read(&L_, "(2 . 3)");
    EXPECT_EQ(2, L_.NumVal(L_.Car(obj)));
    EXPECT_EQ(3, L_.NumVal(L_.Cdr(obj)));
}

// +---+  +---+
// |*|*|->|4|/|
// +---+  +---+
//  |
//  v
// +---+
// |2|3|
// +---+
//
TEST_F(LispReaderTest, ReadDottedPairWithinList)
{
    auto obj = reader_.Read(&L_, "((2 . 3) 4)");
    EXPECT_EQ(2, L_.NumVal(L_.Car(L_.Car(obj))));
    EXPECT_EQ(3, L_.NumVal(L_.Cdr(L_.Car(obj))));
    EXPECT_EQ(4, L_.NumVal(L_.Car(L_.Cdr(obj))));
    EXPECT_TRUE(L_.Null(L_.Cdr(L_.Cdr(obj))));
}

// +---+  +---+
// |2|*|->|3|4|
// +---+  +---+
//
TEST_F(LispReaderTest, ReadDottedPairAtEndOfList)
{
    auto obj = reader_.Read(&L_, "(2 3 . 4)");
    EXPECT_EQ(2, L_.NumVal(L_.Car(obj)));
    EXPECT_EQ(3, L_.NumVal(L_.Car(L_.Cdr(obj))));
    EXPECT_EQ(4, L_.NumVal(L_.Cdr(L_.Cdr(obj))));
}

TEST_F(LispReaderTest, ReaderCanDoMultipleSerialReads)
{
    auto a = reader_.Read(&L_, "HELLO");
    EXPECT_EQ("HELLO", L_.SymbolName(a));
    auto b = reader_.Read(&L_, "42");
    EXPECT_EQ(42, L_.NumVal(b));
    auto c = reader_.Read(&L_, "(LIST 10 20)");
    EXPECT_EQ("LIST", L_.SymbolName(L_.Car(c)));
    EXPECT_EQ(10, L_.NumVal(L_.Car(L_.Cdr(c))));
    EXPECT_EQ(20, L_.NumVal(L_.Car(L_.Cdr(L_.Cdr(c)))));
}

TEST_F(LispReaderTest, ReadTrueKeyword)
{
    auto obj = reader_.Read(&L_, "true");
    EXPECT_EQ(procdraw::LispObjectType::Boolean, L_.TypeOf(obj));
    EXPECT_TRUE(L_.BoolVal(obj));
}

TEST_F(LispReaderTest, ReadFalseKeyword)
{
    auto obj = reader_.Read(&L_, "false");
    EXPECT_EQ(procdraw::LispObjectType::Boolean, L_.TypeOf(obj));
    EXPECT_FALSE(L_.BoolVal(obj));
}

TEST_F(LispReaderTest, ReadNonEmptyString)
{
    auto obj = reader_.Read(&L_, "\"some string\"");
    EXPECT_EQ(procdraw::LispObjectType::String, L_.TypeOf(obj));
    EXPECT_EQ("some string", L_.StringVal(obj));
}

TEST_F(LispReaderTest, ReadEmptyString)
{
    auto obj = reader_.Read(&L_, "\"\"");
    EXPECT_EQ(procdraw::LispObjectType::String, L_.TypeOf(obj));
    EXPECT_EQ("", L_.StringVal(obj));
}

TEST_F(LispReaderTest, ReadListOfStringsAndNumbers)
{
    auto obj = reader_.Read(&L_, "(\"some string\" 42 \"\" 2)");
    EXPECT_EQ("some string", L_.StringVal(L_.Car(obj)));
    EXPECT_EQ(42, L_.NumVal(L_.Cadr(obj)));
    EXPECT_EQ("", L_.StringVal(L_.Caddr(obj)));
    EXPECT_EQ(2, L_.NumVal(L_.Cadddr(obj)));
}

TEST_F(LispReaderTest, ReadingNonClosedStringThrowsException)
{
    EXPECT_THROW(reader_.Read(&L_, "\""), std::runtime_error);
    EXPECT_THROW(reader_.Read(&L_, "\"a"), std::runtime_error);
}

TEST_F(LispReaderTest, ReadingIncompleteExpressionReturnsEof)
{
    EXPECT_EQ(procdraw::LispObjectType::Eof, L_.TypeOf(reader_.Read(&L_, "")));
    EXPECT_EQ(procdraw::LispObjectType::Eof, L_.TypeOf(reader_.Read(&L_, "(")));
    EXPECT_EQ(procdraw::LispObjectType::Eof,
              L_.TypeOf(reader_.Read(&L_, "(1")));
    EXPECT_EQ(procdraw::LispObjectType::Eof,
              L_.TypeOf(reader_.Read(&L_, "(1 2")));
    EXPECT_EQ(procdraw::LispObjectType::Eof,
              L_.TypeOf(reader_.Read(&L_, "(1 (2")));
}

TEST_F(LispReaderTest, CheckBalancedExpression)
{
    EXPECT_EQ(procdraw::BalancedState::Balanced, reader_.CheckBalanced(""));
    EXPECT_EQ(procdraw::BalancedState::Balanced, reader_.CheckBalanced("42"));
    EXPECT_EQ(procdraw::BalancedState::Balanced, reader_.CheckBalanced("()"));
    EXPECT_EQ(procdraw::BalancedState::Balanced,
              reader_.CheckBalanced("(A B C)"));
    EXPECT_EQ(procdraw::BalancedState::Balanced,
              reader_.CheckBalanced("(A (1 2))"));
    EXPECT_EQ(procdraw::BalancedState::Balanced, reader_.CheckBalanced("\"\""));
    EXPECT_EQ(procdraw::BalancedState::Balanced,
              reader_.CheckBalanced("\"abc\""));
}

TEST_F(LispReaderTest, CheckNonClosedExpression)
{
    EXPECT_EQ(procdraw::BalancedState::NotClosed, reader_.CheckBalanced("("));
    EXPECT_EQ(procdraw::BalancedState::NotClosed, reader_.CheckBalanced("(("));
    EXPECT_EQ(procdraw::BalancedState::NotClosed, reader_.CheckBalanced("(42"));
    EXPECT_EQ(procdraw::BalancedState::NotClosed,
              reader_.CheckBalanced("(A B C"));
    EXPECT_EQ(procdraw::BalancedState::NotClosed,
              reader_.CheckBalanced("(A (1 2"));
    EXPECT_EQ(procdraw::BalancedState::NotClosed,
              reader_.CheckBalanced("(A (1 2)"));
}

TEST_F(LispReaderTest, CheckNonClosedString)
{
    EXPECT_EQ(procdraw::BalancedState::NotClosed, reader_.CheckBalanced("\""));
    EXPECT_EQ(procdraw::BalancedState::NotClosed,
              reader_.CheckBalanced("\"abc"));
    EXPECT_EQ(procdraw::BalancedState::NotClosed, reader_.CheckBalanced("(\""));
    EXPECT_EQ(procdraw::BalancedState::NotClosed,
              reader_.CheckBalanced("(42 \""));
    EXPECT_EQ(procdraw::BalancedState::NotClosed,
              reader_.CheckBalanced("(42 (\""));
}

TEST_F(LispReaderTest, CheckTooManyClosingParens)
{
    EXPECT_EQ(procdraw::BalancedState::TooManyClosingParens,
              reader_.CheckBalanced(")"));
    EXPECT_EQ(procdraw::BalancedState::TooManyClosingParens,
              reader_.CheckBalanced("))"));
    EXPECT_EQ(procdraw::BalancedState::TooManyClosingParens,
              reader_.CheckBalanced("(42))"));
    EXPECT_EQ(procdraw::BalancedState::TooManyClosingParens,
              reader_.CheckBalanced("(A B C))"));
    EXPECT_EQ(procdraw::BalancedState::TooManyClosingParens,
              reader_.CheckBalanced("(A (1 2)))"));
}
