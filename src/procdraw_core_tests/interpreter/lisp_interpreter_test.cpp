#include "procdraw_core/interpreter/lisp_interpreter.h"
#include "gtest/gtest.h"
#include <cmath>

class LispInterpreterTest : public ::testing::Test
{
protected:
  procdraw::LispInterpreter L_;
};

TEST_F(LispInterpreterTest, MakeListWithEmptyList)
{
  EXPECT_TRUE(L_.Null(L_.MakeList({})));
}

TEST_F(LispInterpreterTest, MakeListWithSingleItem)
{
  auto list1 = L_.MakeList({ L_.MakeNumber(42) });
  EXPECT_EQ(procdraw::LispObjectType::Cons, L_.TypeOf(list1));
  EXPECT_EQ(42, L_.NumVal(L_.Car(list1)));
  EXPECT_TRUE(L_.Null(L_.Cdr(list1)));
}

// +---+  +---+  +---+  +---+
// |1|*|->|2|*|->|3|*|->|4|/|
// +---+  +---+  +---+  +---+
//
TEST_F(LispInterpreterTest, MakeListWithMultipleItems)
{
  auto list1 = L_.MakeList(
    { L_.MakeNumber(1), L_.MakeNumber(2), L_.MakeNumber(3), L_.MakeNumber(4) });
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
TEST_F(LispInterpreterTest, MakeListWithEmbeddedList)
{
  auto list1 = L_.MakeList({ L_.MakeList({ L_.MakeNumber(1), L_.MakeNumber(2) }),
                            L_.MakeNumber(3), L_.MakeNumber(4) });
  EXPECT_EQ(procdraw::LispObjectType::Cons, L_.TypeOf(list1));
  EXPECT_EQ(1, L_.NumVal(L_.Car(L_.Car(list1))));
  EXPECT_EQ(2, L_.NumVal(L_.Car(L_.Cdr(L_.Car(list1)))));
  EXPECT_TRUE(L_.Null(L_.Cdr(L_.Cdr(L_.Car(list1)))));
  EXPECT_EQ(3, L_.NumVal(L_.Car(L_.Cdr(list1))));
  EXPECT_EQ(4, L_.NumVal(L_.Car(L_.Cdr(L_.Cdr(list1)))));
  EXPECT_TRUE(L_.Null(L_.Cdr(L_.Cdr(L_.Cdr(list1)))));
}

TEST_F(LispInterpreterTest, PrettyPrintToString)
{
  EXPECT_EQ("(42)", L_.PrettyPrintToString(L_.MakeList({ L_.MakeNumber(42) }), 100));
}

TEST_F(LispInterpreterTest, PrintToString)
{
  EXPECT_EQ("(42)", L_.PrintToString(L_.MakeList({ L_.MakeNumber(42) })));
}

TEST_F(LispInterpreterTest, SymbolRef)
{
  auto hello1 = L_.SymbolRef("HELLO");
  EXPECT_EQ(procdraw::LispObjectType::Symbol, L_.TypeOf(hello1));
  EXPECT_EQ("HELLO", L_.SymbolName(hello1));
  auto hello2 = L_.SymbolRef("HELLO");
  EXPECT_EQ(procdraw::LispObjectType::Symbol, L_.TypeOf(hello2));
  EXPECT_EQ("HELLO", L_.SymbolName(hello2));
  // Verify that they are the same address
  EXPECT_EQ(hello1.get(), hello2.get());
}

TEST_F(LispInterpreterTest, Eq)
{
  EXPECT_TRUE(LispObjectEq(L_.Nil, L_.Nil));

  EXPECT_TRUE(LispObjectEq(L_.MakeNumber(42), L_.MakeNumber(42)));
  EXPECT_FALSE(LispObjectEq(L_.MakeNumber(42), L_.MakeNumber(1)));
  EXPECT_FALSE(LispObjectEq(L_.MakeNumber(42), L_.Nil));
  EXPECT_FALSE(LispObjectEq(L_.Nil, L_.MakeNumber(42)));

  EXPECT_TRUE(LispObjectEq(L_.MakeString("hello"), L_.MakeString("hello")));
  EXPECT_FALSE(LispObjectEq(L_.MakeString("hello"), L_.MakeString("world")));
  EXPECT_FALSE(LispObjectEq(L_.MakeString("hello"), L_.Nil));
  EXPECT_FALSE(LispObjectEq(L_.Nil, L_.MakeString("hello")));

  EXPECT_TRUE(LispObjectEq(L_.SymbolRef("A"), L_.SymbolRef("A")));
  EXPECT_FALSE(LispObjectEq(L_.SymbolRef("A"), L_.SymbolRef("B")));

  auto list1 = L_.MakeList({ L_.MakeNumber(42) });
  EXPECT_TRUE(LispObjectEq(list1, list1));
}

TEST_F(LispInterpreterTest, AssocOnEmptyAssociationList)
{
  EXPECT_TRUE(L_.Null(L_.Assoc(L_.SymbolRef("A"), L_.Nil)));
}

TEST_F(LispInterpreterTest, AssocOnNonEmptyAssociationList)
{
  auto symbolA = L_.SymbolRef("A");
  auto symbolB = L_.SymbolRef("B");
  auto alist = L_.MakeList(
    { L_.Cons(symbolA, L_.MakeNumber(1)), L_.Cons(symbolB, L_.MakeNumber(2)) });

  auto pairA = L_.Assoc(symbolA, alist);
  EXPECT_TRUE(LispObjectEq(symbolA, L_.Car(pairA)));
  EXPECT_EQ(1, L_.NumVal(L_.Cdr(pairA)));

  auto pairB = L_.Assoc(symbolB, alist);
  EXPECT_TRUE(LispObjectEq(symbolB, L_.Car(pairB)));
  EXPECT_EQ(2, L_.NumVal(L_.Cdr(pairB)));

  EXPECT_TRUE(L_.Null(L_.Assoc(L_.SymbolRef("C"), alist)));
}

TEST_F(LispInterpreterTest, Rplaca)
{
  auto cons = L_.Cons(L_.MakeNumber(1), L_.MakeNumber(2));
  auto result = L_.Rplaca(cons, L_.MakeNumber(10));
  EXPECT_EQ(10, L_.NumVal(L_.Car(cons)));
  EXPECT_EQ(2, L_.NumVal(L_.Cdr(cons)));
  EXPECT_TRUE(LispObjectEq(cons, result));
}

TEST_F(LispInterpreterTest, Rplacd)
{
  auto cons = L_.Cons(L_.MakeNumber(1), L_.MakeNumber(2));
  auto result = L_.Rplacd(cons, L_.MakeNumber(20));
  EXPECT_EQ(1, L_.NumVal(L_.Car(cons)));
  EXPECT_EQ(20, L_.NumVal(L_.Cdr(cons)));
  EXPECT_TRUE(LispObjectEq(cons, result));
}

/*

TEST_F(LispInterpreterTest, "Dictionaries")
{

  procdraw::LispInterpreter L;

  auto dict = L_.MakeDict();

  TEST_F(LispInterpreterTest, "notFound is returned when get from empty Dictionary")
  {
    EXPECT_TRUE(L_.Null(L_.Get(L_.SymbolRef("key1"), dict, L_.Nil)));
    EXPECT_EQ(L_.NumVal(L_.Get(L_.SymbolRef("key1"), dict, L_.MakeNumber(100))) ==
            100);
    EXPECT_TRUE(L_.Null(L_.Get(L_.SymbolRef("key1"), dict)));
  }

  TEST_F(LispInterpreterTest, "Get, Put, and Clear with symbol keys")
  {
    EXPECT_TRUE(L_.Null(L_.Get(L_.SymbolRef("key1"), dict)));
    EXPECT_EQ(L_.NumVal(L_.Put(L_.SymbolRef("key1"), L_.MakeNumber(42), dict)) == 42);
    EXPECT_EQ(L_.NumVal(L_.Get(L_.SymbolRef("key1"), dict)) == 42);
    EXPECT_EQ(L_.NumVal(L_.Put(L_.SymbolRef("key1"), L_.MakeNumber(10), dict)) == 10);
    EXPECT_EQ(L_.NumVal(L_.Get(L_.SymbolRef("key1"), dict)) == 10);
    EXPECT_TRUE(L_.Null(L_.Get(L_.SymbolRef("key2"), dict)));
    L_.Put(L_.SymbolRef("key2"), L_.MakeNumber(100), dict);
    EXPECT_EQ(L_.NumVal(L_.Get(L_.SymbolRef("key2"), dict)) == 100);
    EXPECT_EQ(L_.NumVal(L_.Get(L_.SymbolRef("key1"), dict)) == 10);
    L_.Clear(dict);
    EXPECT_TRUE(L_.Null(L_.Get(L_.SymbolRef("key1"), dict)));
    EXPECT_TRUE(L_.Null(L_.Get(L_.SymbolRef("key2"), dict)));
  }

  TEST_F(LispInterpreterTest, "String keys")
  {
    EXPECT_TRUE(L_.Null(L_.Get(L_.MakeString("key1"), dict)));
    EXPECT_EQ(L_.NumVal(L_.Put(L_.MakeString("key1"), L_.MakeNumber(42), dict)) ==
            42);
    EXPECT_EQ(L_.NumVal(L_.Get(L_.MakeString("key1"), dict)) == 42);
    EXPECT_EQ(L_.NumVal(L_.Put(L_.MakeString("key1"), L_.MakeNumber(10), dict)) ==
            10);
    EXPECT_EQ(L_.NumVal(L_.Get(L_.MakeString("key1"), dict)) == 10);
    EXPECT_TRUE(L_.Null(L_.Get(L_.MakeString("key2"), dict)));
  }

  TEST_F(LispInterpreterTest, "Keys")
  {
    EXPECT_TRUE(L_.Null(L_.Keys(dict)));

    L_.Put(L_.SymbolRef("key1"), L_.MakeNumber(42), dict);
    auto singleKey = L_.Keys(dict);
    EXPECT_EQ(L_.SymbolName(L_.Car(singleKey)) == "key1");
    EXPECT_TRUE(L_.Null(L_.Cdr(singleKey)));

    L_.Put(L_.SymbolRef("key2"), L_.MakeNumber(10), dict);
    auto twoKeys = L_.Keys(dict);
    EXPECT_TRUE(L_.Null(L_.Cddr(twoKeys)));
    auto foundExpectedKeys = (L_.SymbolName(L_.Car(twoKeys)) == "key1" &&
                              L_.SymbolName(L_.Cadr(twoKeys)) == "key2") ||
                             (L_.SymbolName(L_.Car(twoKeys)) == "key2" &&
                              L_.SymbolName(L_.Cadr(twoKeys)) == "key1");
    EXPECT_TRUE(foundExpectedKeys);
  }
}

TEST_F(LispInterpreterTest, "LispInterpreter implicit type conversion")
{

  procdraw::LispInterpreter L;

  TEST_F(LispInterpreterTest, "to number")
  {
    TEST_F(LispInterpreterTest, "should preserve numbers and convert non-numbers to NaN")
    {
      // Nil
      EXPECT_TRUE(std::isnan(L_.NumVal(L_.Nil)));
      // Number
      EXPECT_EQ(L_.NumVal(L_.MakeNumber(42)) == 42);
      // Symbol
      EXPECT_TRUE(std::isnan(L_.NumVal(L_.SymbolRef("SYMBOL"))));
      // Cons
      EXPECT_TRUE(std::isnan(L_.NumVal(L_.Cons(L_.MakeNumber(1), L_.MakeNumber(2)))));
      // CFunction
      EXPECT_TRUE(std::isnan(L_.NumVal(L_.MakeCFunction(nullptr, nullptr))));
      // Boolean
      EXPECT_TRUE(std::isnan(L_.NumVal(L_.True)));
      EXPECT_TRUE(std::isnan(L_.NumVal(L_.False)));
      // String
      EXPECT_TRUE(std::isnan(L_.NumVal(L_.MakeString("some string"))));
      // Dictionary
      EXPECT_TRUE(std::isnan(L_.NumVal(L_.MakeDict())));
      // Eof
      EXPECT_TRUE(std::isnan(L_.NumVal(L_.Eof)));
    }
  }

  TEST_F(LispInterpreterTest, "to bool")
  {
    TEST_F(LispInterpreterTest, "should preserve booleans, convert nil to false, and other "
            "non-booleans to true")
    {
      // Nil
      EXPECT_FALSE(L_.BoolVal(L_.Nil));
      // Number
      EXPECT_TRUE(L_.BoolVal(L_.MakeNumber(42)));
      // Symbol
      EXPECT_TRUE(L_.BoolVal(L_.SymbolRef("SYMBOL")));
      // Cons
      EXPECT_TRUE(L_.BoolVal(L_.Cons(L_.MakeNumber(1), L_.MakeNumber(2))));
      // CFunction
      EXPECT_TRUE(L_.BoolVal(L_.MakeCFunction(nullptr, nullptr)));
      // Boolean
      EXPECT_TRUE(L_.BoolVal(L_.True));
      EXPECT_FALSE(L_.BoolVal(L_.False));
      // String
      EXPECT_TRUE(L_.BoolVal(L_.MakeString("some string")));
      // Dictionary
      EXPECT_TRUE(L_.BoolVal(L_.MakeDict()));
      // Eof
      EXPECT_TRUE(L_.BoolVal(L_.Eof));
    }
  }
}

TEST_F(LispInterpreterTest, "LispInterpreter::Eval")
{

  // TODO Rework these tests as lists of [input, expected output, expected type]
  // For each input, call L_.PrintToString(L_.Eval(L_.Read(input))) and check the
  // result.
  // It would also be really good to connect these test cases with documentation
  // for each function: either store the test cases in the documentation and
  // extract, or store in some machine readable format and generate parts of the
  // documentation from them.
  // Like Python's doctest: https://en.wikipedia.org/wiki/Doctest

  procdraw::LispInterpreter L;

  TEST_F(LispInterpreterTest, "NIL evaluates to itself") { EXPECT_TRUE(L_.Null(L_.Eval(L_.Nil))); }

  TEST_F(LispInterpreterTest, "A number evaluates to itself")
  {
    EXPECT_EQ(L_.NumVal(L_.Eval(L_.MakeNumber(42))) == 42);
  }

  TEST_F(LispInterpreterTest, "Booleans evaluate to themselves")
  {
    EXPECT_TRUE(L_.BoolVal(L_.Eval(L_.True)));
    EXPECT_FALSE(L_.BoolVal(L_.Eval(L_.False)));
  }

  TEST_F(LispInterpreterTest, "An unbound symbol evaluates to nil")
  {
    EXPECT_TRUE(L_.Null(L_.Eval(L_.SymbolRef("a"))));
  }

  TEST_F(LispInterpreterTest, "A bound symbol evaluates to the bound value")
  {
    auto env = L_.MakeList({ L_.Cons(L_.SymbolRef("a"), L_.MakeNumber(42)) });
    EXPECT_EQ(L_.NumVal(L_.Eval(L_.SymbolRef("a"), env)) == 42);
  }

  TEST_F(LispInterpreterTest, "A string evaluates to itself")
  {
    EXPECT_EQ(L_.StringVal(L_.Eval(L_.MakeString("some string"))) == "some string");
  }

  TEST_F(LispInterpreterTest, "Eof evaluates to itself") { EXPECT_TRUE(L_.IsEof(L_.Eval(L_.Eof))); }

  TEST_F(LispInterpreterTest, "A QUOTE form evaluates to its argument, unevaluated")
  {
    EXPECT_EQ(L_.SymbolName(L_.Eval(L_.Read("(quote foo)"))) == "foo");
    EXPECT_EQ(L_.SymbolName(L_.Eval(L_.Read("'foo"))) == "foo");
  }

  TEST_F(LispInterpreterTest, "SUM of zero arguments is 0")
  {
    EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(+)"))) == 0);
  }

  TEST_F(LispInterpreterTest, "SUM of one or more arguments is the sum of all arguments")
  {
    EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(+ 0)"))) == 0);
    EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(+ 2)"))) == 2);
    EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(+ 2 3)"))) == 5);
    EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(+ 2 3 4)"))) == 9);
  }

  TEST_F(LispInterpreterTest, "SUM works with subexpressions")
  {
    auto env = L_.MakeList({ L_.Cons(L_.SymbolRef("a"), L_.MakeNumber(1)),
                            L_.Cons(L_.SymbolRef("b"), L_.MakeNumber(2)),
                            L_.Cons(L_.SymbolRef("c"), L_.MakeNumber(4)) });
    EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(+ (+ a b 8) 16 c)"), env)) == 31);
  }

  TEST_F(LispInterpreterTest, "DIFFERENCE of zero arguments is 0")
  {
    EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(-)"))) == 0);
  }

  TEST_F(LispInterpreterTest, "DIFFERENCE of one argument is the negation of the argument")
  {
    EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(- 0)"))) == 0);
    EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(- 2)"))) == -2);
    EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(- -2)"))) == 2);
  }

  TEST_F(LispInterpreterTest, "DIFFERENCE of two or more arguments is the first minus the others")
  {
    EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(- 5 2)"))) == 3);
    EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(- 5 2 7)"))) == -4);
  }

  TEST_F(LispInterpreterTest, "PRODUCT of zero arguments is 1")
  {
    EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(*)"))) == 1);
  }

  TEST_F(LispInterpreterTest, "PRODUCT of one or more arguments is the product of all arguments")
  {
    EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(* 0)"))) == 0);
    EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(* 1)"))) == 1);
    EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(* 2)"))) == 2);
    EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(* 2 3)"))) == 6);
    EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(* 2 3 4)"))) == 24);
  }

  TEST_F(LispInterpreterTest, "QUOTIENT of zero arguments is 1")
  {
    EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(/)"))) == 1);
  }

  TEST_F(LispInterpreterTest, "QUOTIENT of one argument is its reciprocal")
  {
    EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(/ 2)"))) == 0.5);
  }

  TEST_F(LispInterpreterTest, "QUOTIENT of 0 is infinity")
  {
    EXPECT_TRUE(std::isinf(L_.NumVal(L_.Eval(L_.Read("(/ 0)")))));
    EXPECT_TRUE(std::isinf(L_.NumVal(L_.Eval(L_.Read("(/ 1 0)")))));
  }

  TEST_F(LispInterpreterTest,
    "QUOTIENT of two or more arguments is the first divided by the others")
  {
    EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(/ 8 5)"))) == 1.6);
    EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(/ 360 4 3)"))) == 30);
  }

  TEST_F(LispInterpreterTest, "A LAMBDA expression evaluates to itself")
  {
    EXPECT_EQ(L_.PrintToString(L_.Eval(L_.Read("(lambda (n) (+ n 1))"))) ==
            "(lambda (n) (+ n 1))");
  }

  TEST_F(LispInterpreterTest, "LAMBDA call no args")
  {
    EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("((lambda () (+ 1 2)))"))) == 3);
  }

  TEST_F(LispInterpreterTest, "LAMBDA call 1 arg")
  {
    EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("((lambda (n) (+ n 1)) 1)"))) == 2);
  }

  TEST_F(LispInterpreterTest, "LAMBDA call 2 args")
  {
    EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("((lambda (m n) (+ m n 10)) 30 2)"))) == 42);
  }

  TEST_F(LispInterpreterTest, "LAMBDA returns the value of the last expression")
  {
    EXPECT_TRUE(L_.Null(L_.Eval(L_.Read("((lambda ()))"))));
    EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("((lambda (n) n) 1)"))) == 1);
    EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("((lambda (n) 1 2 n) 3)"))) == 3);
    EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("((lambda () 1 2 3 (+ 40 2)))"))) == 42);
  }

  TEST_F(LispInterpreterTest, "PROGN returns the value of the last expression")
  {
    EXPECT_TRUE(L_.Null(L_.Eval(L_.Read("(progn)"))));
    EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(progn 1)"))) == 1);
    EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(progn 1 2 3)"))) == 3);
    EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(progn 1 2 3 (+ 40 2))"))) == 42);
  }

  TEST_F(LispInterpreterTest, "SETQ top level")
  {
    auto setqReturn = L_.Eval(L_.Read("(setq a 10)"));
    EXPECT_EQ(L_.NumVal(setqReturn) == 10);
    EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("a"))) == 10);
  }

  TEST_F(LispInterpreterTest, "SETQ in LAMBDA modifies the environment")
  {
    auto setqReturn = L_.Eval(L_.Read("(setq a 1)"));
    EXPECT_EQ(L_.NumVal(setqReturn) == 1);
    L_.Eval(
      L_.Read("(setq f (lambda (a) (progn (setq b a) (setq a 3) (setq c a))))"));
    EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(f 2)"))) == 3);
    EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("a"))) == 1);
    EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("b"))) == 2);
    EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("c"))) == 3);
  }

  TEST_F(LispInterpreterTest, "DEF no args")
  {
    auto defReturn = L_.Eval(L_.Read("(def f () (+ 1 2))"));
    EXPECT_EQ(L_.TypeOf(defReturn) == procdraw::LispObjectType::Cons);
    EXPECT_EQ(L_.SymbolName(L_.Car(defReturn)) == "lambda");
    EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(f)"))) == 3);
  }

  TEST_F(LispInterpreterTest, "DEF 1 arg")
  {
    auto defReturn = L_.Eval(L_.Read("(def f (n) (+ n 1))"));
    EXPECT_EQ(L_.TypeOf(defReturn) == procdraw::LispObjectType::Cons);
    EXPECT_EQ(L_.SymbolName(L_.Car(defReturn)) == "lambda");
    EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(f 1)"))) == 2);
  }

  TEST_F(LispInterpreterTest, "APPLY no args")
  {
    EXPECT_EQ(
      L_.NumVal(L_.Eval(L_.Read("(apply (lambda () (+ 1 2)) (quote ()))"))) == 3);
  }

  TEST_F(LispInterpreterTest, "APPLY 1 arg")
  {
    EXPECT_EQ(L_.NumVal(
              L_.Eval(L_.Read("(apply (lambda (n) (+ n 1)) (quote (1)))"))) == 2);
  }

  TEST_F(LispInterpreterTest, "APPLY 2 args")
  {
    EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read(
              "(apply (lambda (m n) (+ m n 10)) (quote (30 2)))"))) == 42);
  }

  TEST_F(LispInterpreterTest, "CAR")
  {
    EXPECT_TRUE(L_.Null(L_.Eval(L_.Read("(car nil)"))));
    EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(car (quote (1 . 2)))"))) == 1);
  }

  TEST_F(LispInterpreterTest, "CDR")
  {
    EXPECT_TRUE(L_.Null(L_.Eval(L_.Read("(cdr nil)"))));
    EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(cdr (quote (1 . 2)))"))) == 2);
  }

  TEST_F(LispInterpreterTest, "EQ")
  {
    EXPECT_TRUE(L_.BoolVal(L_.Eval(L_.Read("(eq 42 42)"))));
    EXPECT_FALSE(L_.BoolVal(L_.Eval(L_.Read("(eq 1 2)"))));
  }

  TEST_F(LispInterpreterTest, "IF")
  {
    auto env = L_.MakeList({ L_.Cons(L_.SymbolRef("a"), L_.MakeNumber(1)),
                            L_.Cons(L_.SymbolRef("b"), L_.MakeNumber(2)) });

    TEST_F(LispInterpreterTest, "true with else")
    {
      EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(if (eq 42 42) (setq a 10) (setq b 20))"),
                              env)) == 10);
      EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("a"), env)) == 10);
      EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("b"), env)) == 2);
    }

    TEST_F(LispInterpreterTest, "true without else")
    {
      EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(if (eq 42 42) (setq a 10))"), env)) ==
              10);
      EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("a"), env)) == 10);
      EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("b"), env)) == 2);
    }

    TEST_F(LispInterpreterTest, "false with else")
    {
      EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(if (eq 1 2) (setq a 10) (setq b 20))"),
                              env)) == 20);
      EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("a"), env)) == 1);
      EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("b"), env)) == 20);
    }

    TEST_F(LispInterpreterTest, "false without else")
    {
      EXPECT_TRUE(L_.Null(L_.Eval(L_.Read("(if (eq 1 2) (setq a 10))"), env)));
      EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("a"), env)) == 1);
      EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("b"), env)) == 2);
    }
  }

  TEST_F(LispInterpreterTest, "Recursion")
  {
    auto exp = "(progn"
               "  (setq f (lambda (n)"
               "    (if (eq n 0)"
               "      1"
               "      (* n (f (- n 1))))))"
               "  (f 5))";
    EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read(exp))) == 120);
  }

  TEST_F(LispInterpreterTest, "LERP")
  {

    TEST_F(LispInterpreterTest, "interpolates values for [0, 8]")
    {
      EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(lerp 0 8 0)"))) == 0.0);
      EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(lerp 0 8 (/ 4))"))) == 2.0);
      EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(lerp 0 8 (/ 2))"))) == 4.0);
      EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(lerp 0 8 (/ 3 4))"))) == 6.0);
      EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(lerp 0 8 1)"))) == 8.0);
    }

    TEST_F(LispInterpreterTest, "interpolates values for [4, -4]")
    {
      EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(lerp 4 -4 0)"))) == 4.0);
      EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(lerp 4 -4 (/ 4))"))) == 2.0);
      EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(lerp 4 -4 (/ 2 ))"))) == 0.0);
      EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(lerp 4 -4 (/ 3 4))"))) == -2.0);
      EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(lerp 4 -4 1)"))) == -4.0);
    }
  }

  TEST_F(LispInterpreterTest, "MAP-RANGE")
  {

    TEST_F(LispInterpreterTest, "maps values from [0, 10] to [-1, 0]")
    {
      EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(map-range 0 10 -1 0 0)"))) == -1.0);
      EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(map-range 0 10 -1 0 (/ 10 4))"))) ==
              -0.75);
      EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(map-range 0 10 -1 0 5)"))) == -0.5);
      EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(map-range 0 10 -1 0 (/ 30 4))"))) ==
              -0.25);
      EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(map-range 0 10 -1 0 10)"))) == 0.0);
    }

    TEST_F(LispInterpreterTest, "maps values from [0, 10] to [1, -1]")
    {
      EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(map-range 0 10 1 -1 0)"))) == 1.0);
      EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(map-range 0 10 1 -1 (/ 10 4))"))) ==
              0.5);
      EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(map-range 0 10 1 -1 5)"))) == 0.0);
      EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(map-range 0 10 1 -1 (/ 30 4))"))) ==
              -0.5);
      EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(map-range 0 10 1 -1 10)"))) == -1.0);
    }
  }

  TEST_F(LispInterpreterTest, "NORM")
  {

    TEST_F(LispInterpreterTest, "normalizes values for [0, 8]")
    {
      EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(norm 0 8 0)"))) == 0.0);
      EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(norm 0 8 2)"))) == 0.25);
      EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(norm 0 8 4)"))) == 0.5);
      EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(norm 0 8 6)"))) == 0.75);
      EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(norm 0 8 8)"))) == 1.0);
    }

    TEST_F(LispInterpreterTest, "normalizes values for [4, -4]")
    {
      EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(norm 4 -4 4)"))) == 0.0);
      EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(norm 4 -4 2)"))) == 0.25);
      EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(norm 4 -4 0)"))) == 0.5);
      EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(norm 4 -4 -2)"))) == 0.75);
      EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(norm 4 -4 -4)"))) == 1.0);
    }
  }

  TEST_F(LispInterpreterTest, "WRAP")
  {

    TEST_F(LispInterpreterTest, "wraps values for [0, 10]")
    {
      EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(wrap 0 10 0)"))) == 0.0);
      EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(wrap 0 10 10)"))) == 0.0);
      EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(wrap 0 10 20)"))) == 0.0);
      EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(wrap 0 10 -10)"))) == 0.0);
      EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(wrap 0 10 8)"))) == 8.0);
      EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(wrap 0 10 12)"))) == 2.0);
      EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(wrap 0 10 23)"))) == 3.0);
      EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(wrap 0 10 -2)"))) == 8.0);
      EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(wrap 0 10 -13)"))) == 7.0);
    }

    TEST_F(LispInterpreterTest, "wraps values for [-20, -10]")
    {
      EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(wrap -20 -10 -20)"))) == -20.0);
      EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(wrap -20 -10 -10)"))) == -20.0);
      EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(wrap -20 -10 0)"))) == -20.0);
      EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(wrap -20 -10 -30)"))) == -20.0);
      EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(wrap -20 -10 -12)"))) == -12.0);
      EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(wrap -20 -10 -8)"))) == -18.0);
      EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(wrap -20 -10 13)"))) == -17.0);
      EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(wrap -20 -10 -22)"))) == -12.0);
      EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(wrap -20 -10 -33)"))) == -13.0);
    }
  }

  TEST_F(LispInterpreterTest, "ASSOC returns nil if key is not found")
  {
    EXPECT_TRUE(L_.Null(L_.Eval(L_.Read("(assoc 'a '())"))));
    EXPECT_TRUE(L_.Null(L_.Eval(L_.Read("(assoc 'a '((b . 1)))"))));
    EXPECT_TRUE(L_.Null(L_.Eval(L_.Read("(assoc 'a '((b . 1) (c . 2)))"))));
  }

  TEST_F(LispInterpreterTest, "ASSOC returns the first match")
  {
    EXPECT_EQ(L_.PrintToString(L_.Eval(L_.Read("(assoc 'a '((a . 1)))"))) ==
            "(a . 1)");
    EXPECT_EQ(L_.PrintToString(L_.Eval(L_.Read("(assoc 'a '((b . 1) (a . 2)))"))) ==
            "(a . 2)");
    EXPECT_EQ(L_.PrintToString(L_.Eval(
              L_.Read("(assoc 'a '((b . 1) (a . 2) (a . 3)))"))) == "(a . 2)");
  }

  TEST_F(LispInterpreterTest, "PUTASSOC updates the first match, if one is found, and returns val")
  {
    L_.Eval(L_.Read("(setq alist '((a . 1)))"));
    EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(putassoc 'a 2 alist)"))) == 2);
    EXPECT_EQ(L_.PrintToString(L_.Eval(L_.Read("alist"))) == "((a . 2))");
    L_.Eval(L_.Read("(setq alist '((b . 1) (a . 2) (a . 3)))"));
    EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(putassoc 'a 4 alist)"))) == 4);
    EXPECT_EQ(L_.PrintToString(L_.Eval(L_.Read("alist"))) ==
            "((b . 1) (a . 4) (a . 3))");
  }

  TEST_F(LispInterpreterTest, "PUTASSOC adds onto the end if no match is found and returns val")
  {
    L_.Eval(L_.Read("(setq alist '((b . 1)))"));
    EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(putassoc 'a 2 alist)"))) == 2);
    EXPECT_EQ(L_.PrintToString(L_.Eval(L_.Read("alist"))) == "((b . 1) (a . 2))");
    EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(putassoc 'c 3 alist)"))) == 3);
    EXPECT_EQ(L_.PrintToString(L_.Eval(L_.Read("alist"))) ==
            "((b . 1) (a . 2) (c . 3))");
  }

  TEST_F(LispInterpreterTest, "NOT negates the boolean value of an expression")
  {
    EXPECT_EQ(L_.BoolVal(L_.Eval(L_.Read("(not true)"))) == false);
    EXPECT_EQ(L_.BoolVal(L_.Eval(L_.Read("(not false)"))) == true);
    EXPECT_EQ(L_.BoolVal(L_.Eval(L_.Read("(not nil)"))) == true);
    EXPECT_EQ(L_.BoolVal(L_.Eval(L_.Read("(not 0)"))) == false);
    EXPECT_EQ(L_.BoolVal(L_.Eval(L_.Read("(not 1)"))) == false);
    EXPECT_EQ(L_.BoolVal(L_.Eval(L_.Read("(not 2)"))) == false);
    EXPECT_EQ(L_.BoolVal(L_.Eval(L_.Read("(not 'hello)"))) == false);
  }
}

static int testCfunData = 42;

static procdraw::LispObjectPtr
TestCfun(procdraw::LispInterpreter* L, procdraw::LispObjectPtr args,
         procdraw::LispObjectPtr env, void* data)
{
  EXPECT_EQ(data == &testCfunData);
  int dataval = *(static_cast<int*>(data));
  EXPECT_EQ(dataval == 42);
  return L->MakeNumber(L->NumVal(L->Car(args)) + dataval);
}

TEST_F(LispInterpreterTest, "LispInterpreter::SetGlobalCFunction")
{
  procdraw::LispInterpreter L;

  TEST_F(LispInterpreterTest,
    "test-cfun should return the sum of its first argument and testCfunData")
  {
    L_.SetGlobalCFunction("test-cfun", TestCfun, &testCfunData);
    EXPECT_EQ(L_.NumVal(L_.Eval(L_.Read("(test-cfun 10)"))) == 52);
  }
}

*/
