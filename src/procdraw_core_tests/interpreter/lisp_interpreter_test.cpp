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
  auto list1 =
    L_.MakeList({ L_.MakeList({ L_.MakeNumber(1), L_.MakeNumber(2) }),
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
  EXPECT_EQ("(42)",
            L_.PrettyPrintToString(L_.MakeList({ L_.MakeNumber(42) }), 100));
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

TEST_F(LispInterpreterTest, GetWithEmptyDictionary)
{
  auto dict = L_.MakeDict();
  EXPECT_TRUE(L_.Null(L_.Get(L_.SymbolRef("key1"), dict, L_.Nil)));
  EXPECT_EQ(100,
            L_.NumVal(L_.Get(L_.SymbolRef("key1"), dict, L_.MakeNumber(100))));
  EXPECT_TRUE(L_.Null(L_.Get(L_.SymbolRef("key1"), dict)));
}

TEST_F(LispInterpreterTest, GetAndPutAndClearWithSymbolKeys)
{
  auto dict = L_.MakeDict();
  EXPECT_TRUE(L_.Null(L_.Get(L_.SymbolRef("key1"), dict)));
  EXPECT_EQ(42,
            L_.NumVal(L_.Put(L_.SymbolRef("key1"), L_.MakeNumber(42), dict)));
  EXPECT_EQ(42, L_.NumVal(L_.Get(L_.SymbolRef("key1"), dict)));
  EXPECT_EQ(10,
            L_.NumVal(L_.Put(L_.SymbolRef("key1"), L_.MakeNumber(10), dict)));
  EXPECT_EQ(10, L_.NumVal(L_.Get(L_.SymbolRef("key1"), dict)));
  EXPECT_TRUE(L_.Null(L_.Get(L_.SymbolRef("key2"), dict)));
  L_.Put(L_.SymbolRef("key2"), L_.MakeNumber(100), dict);
  EXPECT_EQ(100, L_.NumVal(L_.Get(L_.SymbolRef("key2"), dict)));
  EXPECT_EQ(10, L_.NumVal(L_.Get(L_.SymbolRef("key1"), dict)));
  L_.Clear(dict);
  EXPECT_TRUE(L_.Null(L_.Get(L_.SymbolRef("key1"), dict)));
  EXPECT_TRUE(L_.Null(L_.Get(L_.SymbolRef("key2"), dict)));
}

TEST_F(LispInterpreterTest, GetAndPutWithStringKeys)
{
  auto dict = L_.MakeDict();
  EXPECT_TRUE(L_.Null(L_.Get(L_.MakeString("key1"), dict)));
  EXPECT_EQ(42,
            L_.NumVal(L_.Put(L_.MakeString("key1"), L_.MakeNumber(42), dict)));
  EXPECT_EQ(42, L_.NumVal(L_.Get(L_.MakeString("key1"), dict)));
  EXPECT_EQ(10,
            L_.NumVal(L_.Put(L_.MakeString("key1"), L_.MakeNumber(10), dict)));
  EXPECT_EQ(10, L_.NumVal(L_.Get(L_.MakeString("key1"), dict)));
  EXPECT_TRUE(L_.Null(L_.Get(L_.MakeString("key2"), dict)));
}

TEST_F(LispInterpreterTest, Keys)
{
  auto dict = L_.MakeDict();

  EXPECT_TRUE(L_.Null(L_.Keys(dict)));

  L_.Put(L_.SymbolRef("key1"), L_.MakeNumber(42), dict);
  auto singleKey = L_.Keys(dict);
  EXPECT_EQ("key1", L_.SymbolName(L_.Car(singleKey)));
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

TEST_F(LispInterpreterTest, ImplicitTypeConversionToNumber)
{
  // Nil
  EXPECT_TRUE(std::isnan(L_.NumVal(L_.Nil)));
  // Number
  EXPECT_EQ(42, L_.NumVal(L_.MakeNumber(42)));
  // Symbol
  EXPECT_TRUE(std::isnan(L_.NumVal(L_.SymbolRef("SYMBOL"))));
  // Cons
  EXPECT_TRUE(
    std::isnan(L_.NumVal(L_.Cons(L_.MakeNumber(1), L_.MakeNumber(2)))));
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

TEST_F(LispInterpreterTest, ImplicitTypeConversionToBool)
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

TEST_F(LispInterpreterTest, EvalNil)
{
  EXPECT_TRUE(L_.Null(L_.Eval(L_.Nil)));
}

TEST_F(LispInterpreterTest, EvalNumber)
{
  EXPECT_EQ(42, L_.NumVal(L_.Eval(L_.MakeNumber(42))));
}

TEST_F(LispInterpreterTest, EvalBool)
{
  EXPECT_TRUE(L_.BoolVal(L_.Eval(L_.True)));
  EXPECT_FALSE(L_.BoolVal(L_.Eval(L_.False)));
}

TEST_F(LispInterpreterTest, EvalUnboundSymbol)
{
  EXPECT_TRUE(L_.Null(L_.Eval(L_.SymbolRef("a"))));
}

TEST_F(LispInterpreterTest, EvalBoundSymbol)
{
  auto env = L_.MakeList({ L_.Cons(L_.SymbolRef("a"), L_.MakeNumber(42)) });
  EXPECT_EQ(42, L_.NumVal(L_.Eval(L_.SymbolRef("a"), env)));
}

TEST_F(LispInterpreterTest, EvalString)
{
  EXPECT_EQ("some string", L_.StringVal(L_.Eval(L_.MakeString("some string"))));
}

TEST_F(LispInterpreterTest, EvalEof)
{
  EXPECT_TRUE(L_.IsEof(L_.Eval(L_.Eof)));
}

TEST_F(LispInterpreterTest, EvalQuote)
{
  EXPECT_EQ("foo", L_.SymbolName(L_.Eval(L_.Read("(quote foo)"))));
  EXPECT_EQ("foo", L_.SymbolName(L_.Eval(L_.Read("'foo"))));
}

TEST_F(LispInterpreterTest, EvalSumWithSubexpressions)
{
  auto env = L_.MakeList({ L_.Cons(L_.SymbolRef("a"), L_.MakeNumber(1)),
                           L_.Cons(L_.SymbolRef("b"), L_.MakeNumber(2)),
                           L_.Cons(L_.SymbolRef("c"), L_.MakeNumber(4)) });
  EXPECT_EQ(31, L_.NumVal(L_.Eval(L_.Read("(+ (+ a b 8) 16 c)"), env)));
}

TEST_F(LispInterpreterTest, EvalDifferenceOfZeroArgsIs0)
{
  EXPECT_EQ(0, L_.NumVal(L_.Eval(L_.Read("(-)"))));
}

TEST_F(LispInterpreterTest, EvalQuotientOfZeroArgsIs1)
{
  EXPECT_EQ(1, L_.NumVal(L_.Eval(L_.Read("(/)"))));
}

TEST_F(LispInterpreterTest, EvalQuotientWithDivisorOf0IsInfinity)
{
  EXPECT_TRUE(std::isinf(L_.NumVal(L_.Eval(L_.Read("(/ 0)")))));
  EXPECT_TRUE(std::isinf(L_.NumVal(L_.Eval(L_.Read("(/ 1 0)")))));
}

TEST_F(LispInterpreterTest, EvalLambdaExpressionEvaluatesToSelf)
{
  EXPECT_EQ("(lambda (n) (+ n 1))",
            L_.PrintToString(L_.Eval(L_.Read("(lambda (n) (+ n 1))"))));
}

TEST_F(LispInterpreterTest, EvalLambdaCallNoArgs)
{
  EXPECT_EQ(3, L_.NumVal(L_.Eval(L_.Read("((lambda () (+ 1 2)))"))));
}

TEST_F(LispInterpreterTest, EvalLambdaCallOneArg)
{
  EXPECT_EQ(2, L_.NumVal(L_.Eval(L_.Read("((lambda (n) (+ n 1)) 1)"))));
}

TEST_F(LispInterpreterTest, EvalLambdaCallTwoArgs)
{
  EXPECT_EQ(42,
            L_.NumVal(L_.Eval(L_.Read("((lambda (m n) (+ m n 10)) 30 2)"))));
}

TEST_F(LispInterpreterTest, EvalLambdaReturnsValueOfLastExpression)
{
  EXPECT_TRUE(L_.Null(L_.Eval(L_.Read("((lambda ()))"))));
  EXPECT_EQ(1, L_.NumVal(L_.Eval(L_.Read("((lambda (n) n) 1)"))));
  EXPECT_EQ(3, L_.NumVal(L_.Eval(L_.Read("((lambda (n) 1 2 n) 3)"))));
  EXPECT_EQ(42, L_.NumVal(L_.Eval(L_.Read("((lambda () 1 2 3 (+ 40 2)))"))));
}

TEST_F(LispInterpreterTest, EvalPrognReturnsValueOfLastExpression)
{
  EXPECT_TRUE(L_.Null(L_.Eval(L_.Read("(progn)"))));
  EXPECT_EQ(1, L_.NumVal(L_.Eval(L_.Read("(progn 1)"))));
  EXPECT_EQ(3, L_.NumVal(L_.Eval(L_.Read("(progn 1 2 3)"))));
  EXPECT_EQ(42, L_.NumVal(L_.Eval(L_.Read("(progn 1 2 3 (+ 40 2))"))));
}

TEST_F(LispInterpreterTest, EvalSetqTopLevel)
{
  auto setqReturn = L_.Eval(L_.Read("(setq a 10)"));
  EXPECT_EQ(10, L_.NumVal(setqReturn));
  EXPECT_EQ(10, L_.NumVal(L_.Eval(L_.Read("a"))));
}

TEST_F(LispInterpreterTest, EvalSetqInLambdaModifiesEnvironment)
{
  auto setqReturn = L_.Eval(L_.Read("(setq a 1)"));
  EXPECT_EQ(1, L_.NumVal(setqReturn));
  L_.Eval(
    L_.Read("(setq f (lambda (a) (progn (setq b a) (setq a 3) (setq c a))))"));
  EXPECT_EQ(3, L_.NumVal(L_.Eval(L_.Read("(f 2)"))));
  EXPECT_EQ(1, L_.NumVal(L_.Eval(L_.Read("a"))));
  EXPECT_EQ(2, L_.NumVal(L_.Eval(L_.Read("b"))));
  EXPECT_EQ(3, L_.NumVal(L_.Eval(L_.Read("c"))));
}

TEST_F(LispInterpreterTest, EvalDefNoArgs)
{
  auto defReturn = L_.Eval(L_.Read("(def f () (+ 1 2))"));
  EXPECT_EQ(procdraw::LispObjectType::Cons, L_.TypeOf(defReturn));
  EXPECT_EQ("lambda", L_.SymbolName(L_.Car(defReturn)));
  EXPECT_EQ(3, L_.NumVal(L_.Eval(L_.Read("(f)"))));
}

TEST_F(LispInterpreterTest, EvalDefOneArg)
{
  auto defReturn = L_.Eval(L_.Read("(def f (n) (+ n 1))"));
  EXPECT_EQ(procdraw::LispObjectType::Cons, L_.TypeOf(defReturn));
  EXPECT_EQ("lambda", L_.SymbolName(L_.Car(defReturn)));
  EXPECT_EQ(2, L_.NumVal(L_.Eval(L_.Read("(f 1)"))));
}

TEST_F(LispInterpreterTest, EvalApplyNoArgs)
{
  EXPECT_EQ(
    3, L_.NumVal(L_.Eval(L_.Read("(apply (lambda () (+ 1 2)) (quote ()))"))));
}

TEST_F(LispInterpreterTest, EvalApplyOneArg)
{
  EXPECT_EQ(
    2, L_.NumVal(L_.Eval(L_.Read("(apply (lambda (n) (+ n 1)) (quote (1)))"))));
}

TEST_F(LispInterpreterTest, EvalApplyTwoArgs)
{
  EXPECT_EQ(42, L_.NumVal(L_.Eval(L_.Read(
                  "(apply (lambda (m n) (+ m n 10)) (quote (30 2)))"))));
}

TEST_F(LispInterpreterTest, EvalCar)
{
  EXPECT_TRUE(L_.Null(L_.Eval(L_.Read("(car nil)"))));
  EXPECT_EQ(1, L_.NumVal(L_.Eval(L_.Read("(car (quote (1 . 2)))"))));
}

TEST_F(LispInterpreterTest, EvalCdr)
{
  EXPECT_TRUE(L_.Null(L_.Eval(L_.Read("(cdr nil)"))));
  EXPECT_EQ(2, L_.NumVal(L_.Eval(L_.Read("(cdr (quote (1 . 2)))"))));
}

TEST_F(LispInterpreterTest, EvalIfTrueWithElse)
{
  auto env = L_.MakeList({ L_.Cons(L_.SymbolRef("a"), L_.MakeNumber(1)),
                           L_.Cons(L_.SymbolRef("b"), L_.MakeNumber(2)) });
  EXPECT_EQ(10, L_.NumVal(L_.Eval(
                  L_.Read("(if (eq 42 42) (setq a 10) (setq b 20))"), env)));
  EXPECT_EQ(10, L_.NumVal(L_.Eval(L_.Read("a"), env)));
  EXPECT_EQ(2, L_.NumVal(L_.Eval(L_.Read("b"), env)));
}

TEST_F(LispInterpreterTest, EvalIfTrueWithoutElse)
{
  auto env = L_.MakeList({ L_.Cons(L_.SymbolRef("a"), L_.MakeNumber(1)),
                           L_.Cons(L_.SymbolRef("b"), L_.MakeNumber(2)) });
  EXPECT_EQ(10,
            L_.NumVal(L_.Eval(L_.Read("(if (eq 42 42) (setq a 10))"), env)));
  EXPECT_EQ(10, L_.NumVal(L_.Eval(L_.Read("a"), env)));
  EXPECT_EQ(2, L_.NumVal(L_.Eval(L_.Read("b"), env)));
}

TEST_F(LispInterpreterTest, EvalIfFalseWithElse)
{
  auto env = L_.MakeList({ L_.Cons(L_.SymbolRef("a"), L_.MakeNumber(1)),
                           L_.Cons(L_.SymbolRef("b"), L_.MakeNumber(2)) });
  EXPECT_EQ(20, L_.NumVal(L_.Eval(
                  L_.Read("(if (eq 1 2) (setq a 10) (setq b 20))"), env)));
  EXPECT_EQ(1, L_.NumVal(L_.Eval(L_.Read("a"), env)));
  EXPECT_EQ(20, L_.NumVal(L_.Eval(L_.Read("b"), env)));
}

TEST_F(LispInterpreterTest, EvalIfFalseWithoutElse)
{
  auto env = L_.MakeList({ L_.Cons(L_.SymbolRef("a"), L_.MakeNumber(1)),
                           L_.Cons(L_.SymbolRef("b"), L_.MakeNumber(2)) });
  EXPECT_TRUE(L_.Null(L_.Eval(L_.Read("(if (eq 1 2) (setq a 10))"), env)));
  EXPECT_EQ(1, L_.NumVal(L_.Eval(L_.Read("a"), env)));
  EXPECT_EQ(2, L_.NumVal(L_.Eval(L_.Read("b"), env)));
}

TEST_F(LispInterpreterTest, EvalRecursion)
{
  auto exp = "(progn"
             "  (setq f (lambda (n)"
             "    (if (eq n 0)"
             "      1"
             "      (* n (f (- n 1))))))"
             "  (f 5))";
  EXPECT_EQ(120, L_.NumVal(L_.Eval(L_.Read(exp))));
}

TEST_F(LispInterpreterTest, EvalLerp)
{
  // [0, 8]

  EXPECT_EQ(0.0, L_.NumVal(L_.Eval(L_.Read("(lerp 0 8 0)"))));
  EXPECT_EQ(2.0, L_.NumVal(L_.Eval(L_.Read("(lerp 0 8 (/ 4))"))));
  EXPECT_EQ(4.0, L_.NumVal(L_.Eval(L_.Read("(lerp 0 8 (/ 2))"))));
  EXPECT_EQ(6.0, L_.NumVal(L_.Eval(L_.Read("(lerp 0 8 (/ 3 4))"))));
  EXPECT_EQ(8.0, L_.NumVal(L_.Eval(L_.Read("(lerp 0 8 1)"))));

  // [4, -4]

  EXPECT_EQ(4.0, L_.NumVal(L_.Eval(L_.Read("(lerp 4 -4 0)"))));
  EXPECT_EQ(2.0, L_.NumVal(L_.Eval(L_.Read("(lerp 4 -4 (/ 4))"))));
  EXPECT_EQ(0.0, L_.NumVal(L_.Eval(L_.Read("(lerp 4 -4 (/ 2 ))"))));
  EXPECT_EQ(-2.0, L_.NumVal(L_.Eval(L_.Read("(lerp 4 -4 (/ 3 4))"))));
  EXPECT_EQ(-4.0, L_.NumVal(L_.Eval(L_.Read("(lerp 4 -4 1)"))));
}

TEST_F(LispInterpreterTest, EvalMapRange)
{
  // [0, 10] to [-1, 0]

  EXPECT_EQ(-1.0, L_.NumVal(L_.Eval(L_.Read("(map-range 0 10 -1 0 0)"))));
  EXPECT_EQ(-0.75,
            L_.NumVal(L_.Eval(L_.Read("(map-range 0 10 -1 0 (/ 10 4))"))));
  EXPECT_EQ(-0.5, L_.NumVal(L_.Eval(L_.Read("(map-range 0 10 -1 0 5)"))));
  EXPECT_EQ(-0.25,
            L_.NumVal(L_.Eval(L_.Read("(map-range 0 10 -1 0 (/ 30 4))"))));
  EXPECT_EQ(0.0, L_.NumVal(L_.Eval(L_.Read("(map-range 0 10 -1 0 10)"))));

  // [0, 10] to [1, -1]

  EXPECT_EQ(1.0, L_.NumVal(L_.Eval(L_.Read("(map-range 0 10 1 -1 0)"))));
  EXPECT_EQ(0.5, L_.NumVal(L_.Eval(L_.Read("(map-range 0 10 1 -1 (/ 10 4))"))));
  EXPECT_EQ(0.0, L_.NumVal(L_.Eval(L_.Read("(map-range 0 10 1 -1 5)"))));
  EXPECT_EQ(-0.5,
            L_.NumVal(L_.Eval(L_.Read("(map-range 0 10 1 -1 (/ 30 4))"))));
  EXPECT_EQ(-1.0, L_.NumVal(L_.Eval(L_.Read("(map-range 0 10 1 -1 10)"))));
}

TEST_F(LispInterpreterTest, EvalNorm)
{
  // [0, 8]

  EXPECT_EQ(0.0, L_.NumVal(L_.Eval(L_.Read("(norm 0 8 0)"))));
  EXPECT_EQ(0.25, L_.NumVal(L_.Eval(L_.Read("(norm 0 8 2)"))));
  EXPECT_EQ(0.5, L_.NumVal(L_.Eval(L_.Read("(norm 0 8 4)"))));
  EXPECT_EQ(0.75, L_.NumVal(L_.Eval(L_.Read("(norm 0 8 6)"))));
  EXPECT_EQ(1.0, L_.NumVal(L_.Eval(L_.Read("(norm 0 8 8)"))));

  // [4, -4]

  EXPECT_EQ(0.0, L_.NumVal(L_.Eval(L_.Read("(norm 4 -4 4)"))));
  EXPECT_EQ(0.25, L_.NumVal(L_.Eval(L_.Read("(norm 4 -4 2)"))));
  EXPECT_EQ(0.5, L_.NumVal(L_.Eval(L_.Read("(norm 4 -4 0)"))));
  EXPECT_EQ(0.75, L_.NumVal(L_.Eval(L_.Read("(norm 4 -4 -2)"))));
  EXPECT_EQ(1.0, L_.NumVal(L_.Eval(L_.Read("(norm 4 -4 -4)"))));
}

TEST_F(LispInterpreterTest, EvalWrap)
{
  // [0, 10]

  EXPECT_EQ(0.0, L_.NumVal(L_.Eval(L_.Read("(wrap 0 10 0)"))));
  EXPECT_EQ(0.0, L_.NumVal(L_.Eval(L_.Read("(wrap 0 10 10)"))));
  EXPECT_EQ(0.0, L_.NumVal(L_.Eval(L_.Read("(wrap 0 10 20)"))));
  EXPECT_EQ(0.0, L_.NumVal(L_.Eval(L_.Read("(wrap 0 10 -10)"))));
  EXPECT_EQ(8.0, L_.NumVal(L_.Eval(L_.Read("(wrap 0 10 8)"))));
  EXPECT_EQ(2.0, L_.NumVal(L_.Eval(L_.Read("(wrap 0 10 12)"))));
  EXPECT_EQ(3.0, L_.NumVal(L_.Eval(L_.Read("(wrap 0 10 23)"))));
  EXPECT_EQ(8.0, L_.NumVal(L_.Eval(L_.Read("(wrap 0 10 -2)"))));
  EXPECT_EQ(7.0, L_.NumVal(L_.Eval(L_.Read("(wrap 0 10 -13)"))));

  // [-20, -10]

  EXPECT_EQ(-20.0, L_.NumVal(L_.Eval(L_.Read("(wrap -20 -10 -20)"))));
  EXPECT_EQ(-20.0, L_.NumVal(L_.Eval(L_.Read("(wrap -20 -10 -10)"))));
  EXPECT_EQ(-20.0, L_.NumVal(L_.Eval(L_.Read("(wrap -20 -10 0)"))));
  EXPECT_EQ(-20.0, L_.NumVal(L_.Eval(L_.Read("(wrap -20 -10 -30)"))));
  EXPECT_EQ(-12.0, L_.NumVal(L_.Eval(L_.Read("(wrap -20 -10 -12)"))));
  EXPECT_EQ(-18.0, L_.NumVal(L_.Eval(L_.Read("(wrap -20 -10 -8)"))));
  EXPECT_EQ(-17.0, L_.NumVal(L_.Eval(L_.Read("(wrap -20 -10 13)"))));
  EXPECT_EQ(-12.0, L_.NumVal(L_.Eval(L_.Read("(wrap -20 -10 -22)"))));
  EXPECT_EQ(-13.0, L_.NumVal(L_.Eval(L_.Read("(wrap -20 -10 -33)"))));
}

TEST_F(LispInterpreterTest, EvalAssoc)
{
  EXPECT_TRUE(L_.Null(L_.Eval(L_.Read("(assoc 'a '())"))));
  EXPECT_TRUE(L_.Null(L_.Eval(L_.Read("(assoc 'a '((b . 1)))"))));
  EXPECT_TRUE(L_.Null(L_.Eval(L_.Read("(assoc 'a '((b . 1) (c . 2)))"))));

  EXPECT_EQ("(a . 1)",
            L_.PrintToString(L_.Eval(L_.Read("(assoc 'a '((a . 1)))"))));
  EXPECT_EQ("(a . 2)", L_.PrintToString(
                         L_.Eval(L_.Read("(assoc 'a '((b . 1) (a . 2)))"))));
  EXPECT_EQ("(a . 2)", L_.PrintToString(L_.Eval(
                         L_.Read("(assoc 'a '((b . 1) (a . 2) (a . 3)))"))));
}

TEST_F(LispInterpreterTest, EvalPutassocUpdatesFirstMatch)
{
  L_.Eval(L_.Read("(setq alist '((a . 1)))"));
  EXPECT_EQ(2, L_.NumVal(L_.Eval(L_.Read("(putassoc 'a 2 alist)"))));
  EXPECT_EQ("((a . 2))", L_.PrintToString(L_.Eval(L_.Read("alist"))));
  L_.Eval(L_.Read("(setq alist '((b . 1) (a . 2) (a . 3)))"));
  EXPECT_EQ(4, L_.NumVal(L_.Eval(L_.Read("(putassoc 'a 4 alist)"))));
  EXPECT_EQ("((b . 1) (a . 4) (a . 3))",
            L_.PrintToString(L_.Eval(L_.Read("alist"))));
}

TEST_F(LispInterpreterTest, EvalPutassocNoMatch)
{
  L_.Eval(L_.Read("(setq alist '((b . 1)))"));
  EXPECT_EQ(2, L_.NumVal(L_.Eval(L_.Read("(putassoc 'a 2 alist)"))));
  EXPECT_EQ("((b . 1) (a . 2))", L_.PrintToString(L_.Eval(L_.Read("alist"))));
  EXPECT_EQ(3, L_.NumVal(L_.Eval(L_.Read("(putassoc 'c 3 alist)"))));
  EXPECT_EQ("((b . 1) (a . 2) (c . 3))",
            L_.PrintToString(L_.Eval(L_.Read("alist"))));
}

TEST_F(LispInterpreterTest, EvalNot)
{
  EXPECT_FALSE(L_.BoolVal(L_.Eval(L_.Read("(not true)"))));
  EXPECT_TRUE(L_.BoolVal(L_.Eval(L_.Read("(not false)"))));
  EXPECT_TRUE(L_.BoolVal(L_.Eval(L_.Read("(not nil)"))));
  EXPECT_FALSE(L_.BoolVal(L_.Eval(L_.Read("(not 0)"))));
  EXPECT_FALSE(L_.BoolVal(L_.Eval(L_.Read("(not 1)"))));
  EXPECT_FALSE(L_.BoolVal(L_.Eval(L_.Read("(not 2)"))));
  EXPECT_FALSE(L_.BoolVal(L_.Eval(L_.Read("(not 'hello)"))));
}

static int testCfunData = 42;

static procdraw::LispObjectPtr
TestCfun(procdraw::LispInterpreter* L, procdraw::LispObjectPtr args,
         procdraw::LispObjectPtr env, void* data)
{
  EXPECT_EQ(&testCfunData, data);
  int dataval = *(static_cast<int*>(data));
  EXPECT_EQ(42, dataval);
  return L->MakeNumber(L->NumVal(L->Car(args)) + dataval);
}

TEST_F(LispInterpreterTest, SetGlobalCFunction)
{
  L_.SetGlobalCFunction("test-cfun", TestCfun, &testCfunData);
  EXPECT_EQ(52, L_.NumVal(L_.Eval(L_.Read("(test-cfun 10)"))));
}
