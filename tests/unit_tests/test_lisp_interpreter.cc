#include "lisp_interpreter.h"
#include "catch.hpp"
#include <cmath>

TEST_CASE("LispInterpreter::MakeList()")
{

    procdraw::LispInterpreter L;

    SECTION("Empty list") {
        REQUIRE(L.Null(L.MakeList({})));
    }

    SECTION("Single item") {
        auto list1 = L.MakeList({ L.MakeNumber(42) });
        REQUIRE(L.TypeOf(list1) == procdraw::LispObjectType::Cons);
        REQUIRE(L.NumVal(L.Car(list1)) == 42);
        REQUIRE(L.Null(L.Cdr(list1)));
    }

    // +---+  +---+  +---+  +---+
    // |1|*|->|2|*|->|3|*|->|4|/|
    // +---+  +---+  +---+  +---+
    //
    SECTION("Multiple items") {
        auto list1 = L.MakeList({ L.MakeNumber(1), L.MakeNumber(2), L.MakeNumber(3), L.MakeNumber(4) });
        REQUIRE(L.TypeOf(list1) == procdraw::LispObjectType::Cons);
        REQUIRE(L.NumVal(L.Car(list1)) == 1);
        REQUIRE(L.NumVal(L.Car(L.Cdr(list1))) == 2);
        REQUIRE(L.NumVal(L.Car(L.Cdr(L.Cdr(list1)))) == 3);
        REQUIRE(L.NumVal(L.Car(L.Cdr(L.Cdr(L.Cdr(list1))))) == 4);
        REQUIRE(L.Null(L.Cdr(L.Cdr(L.Cdr(L.Cdr(list1))))));
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
    SECTION("Embedded list") {
        auto list1 = L.MakeList({ L.MakeList({ L.MakeNumber(1), L.MakeNumber(2) }), L.MakeNumber(3), L.MakeNumber(4) });
        REQUIRE(L.TypeOf(list1) == procdraw::LispObjectType::Cons);
        REQUIRE(L.NumVal(L.Car(L.Car(list1))) == 1);
        REQUIRE(L.NumVal(L.Car(L.Cdr(L.Car(list1)))) == 2);
        REQUIRE(L.Null(L.Cdr(L.Cdr(L.Car(list1)))));
        REQUIRE(L.NumVal(L.Car(L.Cdr(list1))) == 3);
        REQUIRE(L.NumVal(L.Car(L.Cdr(L.Cdr(list1)))) == 4);
        REQUIRE(L.Null(L.Cdr(L.Cdr(L.Cdr(list1)))));
    }
}

TEST_CASE("LispInterpreter::PrintToString()")
{
    procdraw::LispInterpreter L;

    REQUIRE(L.PrintToString(L.MakeList({ L.MakeNumber(42) })) == "(42)");
}

TEST_CASE("LispInterpreter::SymbolRef()")
{

    procdraw::LispInterpreter L;

    auto hello1 = L.SymbolRef("HELLO");
    REQUIRE(L.TypeOf(hello1) == procdraw::LispObjectType::Symbol);
    REQUIRE(L.SymbolName(hello1) == "HELLO");
    auto hello2 = L.SymbolRef("HELLO");
    REQUIRE(L.TypeOf(hello2) == procdraw::LispObjectType::Symbol);
    REQUIRE(L.SymbolName(hello2) == "HELLO");
    // Verify that they are the same address
    REQUIRE(hello1.get() == hello2.get());
}

TEST_CASE("LispInterpreter::Eq()")
{

    procdraw::LispInterpreter L;

    REQUIRE(L.Eq(L.Nil, L.Nil));

    REQUIRE(L.Eq(L.MakeNumber(42), L.MakeNumber(42)));
    REQUIRE_FALSE(L.Eq(L.MakeNumber(42), L.MakeNumber(1)));
    REQUIRE_FALSE(L.Eq(L.MakeNumber(42), L.Nil));
    REQUIRE_FALSE(L.Eq(L.Nil, L.MakeNumber(42)));

    REQUIRE(L.Eq(L.SymbolRef("A"), L.SymbolRef("A")));
    REQUIRE_FALSE(L.Eq(L.SymbolRef("A"), L.SymbolRef("B")));

    auto list1 = L.MakeList({ L.MakeNumber(42) });
    REQUIRE(L.Eq(list1, list1));
}

TEST_CASE("LispInterpreter::Assoc()")
{

    procdraw::LispInterpreter L;

    SECTION("Empty association list") {
        REQUIRE(L.Null(L.Assoc(L.SymbolRef("A"), L.Nil)));
    }

    SECTION("Non-empty association list") {
        auto symbolA = L.SymbolRef("A");
        auto symbolB = L.SymbolRef("B");
        auto alist = L.MakeList({ L.Cons(symbolA, L.MakeNumber(1)), L.Cons(symbolB, L.MakeNumber(2)) });

        auto pairA = L.Assoc(symbolA, alist);
        REQUIRE(L.Eq(symbolA, L.Car(pairA)));
        REQUIRE(L.NumVal(L.Cdr(pairA)) == 1);

        auto pairB = L.Assoc(symbolB, alist);
        REQUIRE(L.Eq(symbolB, L.Car(pairB)));
        REQUIRE(L.NumVal(L.Cdr(pairB)) == 2);

        REQUIRE(L.Null(L.Assoc(L.SymbolRef("C"), alist)));
    }

}

TEST_CASE("LispInterpreter::Rplaca()")
{

    procdraw::LispInterpreter L;

    auto cons = L.Cons(L.MakeNumber(1), L.MakeNumber(2));
    auto result = L.Rplaca(cons, L.MakeNumber(10));
    REQUIRE(L.NumVal(L.Car(cons)) == 10);
    REQUIRE(L.NumVal(L.Cdr(cons)) == 2);
    REQUIRE(L.Eq(cons, result));
}

TEST_CASE("LispInterpreter::Rplacd()")
{

    procdraw::LispInterpreter L;

    auto cons = L.Cons(L.MakeNumber(1), L.MakeNumber(2));
    auto result = L.Rplacd(cons, L.MakeNumber(20));
    REQUIRE(L.NumVal(L.Car(cons)) == 1);
    REQUIRE(L.NumVal(L.Cdr(cons)) == 20);
    REQUIRE(L.Eq(cons, result));
}

TEST_CASE("LispInterpreter implicit type conversion")
{

    procdraw::LispInterpreter L;

    SECTION("to number") {
        SECTION("should preserve numbers and convert non-numbers to NaN") {
            // Nil
            REQUIRE(std::isnan(L.NumVal(L.Nil)));
            // Number
            REQUIRE(L.NumVal(L.MakeNumber(42)) == 42);
            // Symbol
            REQUIRE(std::isnan(L.NumVal(L.SymbolRef("SYMBOL"))));
            // Cons
            REQUIRE(std::isnan(L.NumVal(L.Cons(L.MakeNumber(1), L.MakeNumber(2)))));
            // CFunction
            REQUIRE(std::isnan(L.NumVal(L.MakeCFunction(nullptr, nullptr))));
            // Boolean
            REQUIRE(std::isnan(L.NumVal(L.True)));
            REQUIRE(std::isnan(L.NumVal(L.False)));
            // String
            REQUIRE(std::isnan(L.NumVal(L.MakeString("some string"))));
            // Eof
            REQUIRE(std::isnan(L.NumVal(L.Eof)));
        }
    }

    SECTION("to bool") {
        SECTION("should preserve booleans, convert nil to false, and other non-booleans to true") {
            // Nil
            REQUIRE_FALSE(L.BoolVal(L.Nil));
            // Number
            REQUIRE(L.BoolVal(L.MakeNumber(42)));
            // Symbol
            REQUIRE(L.BoolVal(L.SymbolRef("SYMBOL")));
            // Cons
            REQUIRE(L.BoolVal(L.Cons(L.MakeNumber(1), L.MakeNumber(2))));
            // CFunction
            REQUIRE(L.BoolVal(L.MakeCFunction(nullptr, nullptr)));
            // Boolean
            REQUIRE(L.BoolVal(L.True));
            REQUIRE_FALSE(L.BoolVal(L.False));
            // String
            REQUIRE(L.BoolVal(L.MakeString("some string")));
            // Eof
            REQUIRE(L.BoolVal(L.Eof));
        }
    }

}

TEST_CASE("LispInterpreter::Eval")
{

    // TODO Rework these tests as lists of [input, expected output, expected type]
    // For each input, call L.PrintToString(L.Eval(L.Read(input))) and check the result.
    // It would also be really good to connect these test cases with documentation
    // for each function: either store the test cases in the documentation and
    // extract, or store in some machine readable format and generate parts of the
    // documentation from them.
    // Like Python's doctest: https://en.wikipedia.org/wiki/Doctest

    procdraw::LispInterpreter L;

    SECTION("NIL evaluates to itself") {
        REQUIRE(L.Null(L.Eval(L.Nil)));
    }

    SECTION("A number evaluates to itself") {
        REQUIRE(L.NumVal(L.Eval(L.MakeNumber(42))) == 42);
    }

    SECTION("Booleans evaluate to themselves") {
        REQUIRE(L.BoolVal(L.Eval(L.True)));
        REQUIRE_FALSE(L.BoolVal(L.Eval(L.False)));
    }

    SECTION("An unbound symbol evaluates to nil") {
        REQUIRE(L.Null(L.Eval(L.SymbolRef("a"))));
    }

    SECTION("A bound symbol evaluates to the bound value") {
        auto env = L.MakeList({ L.Cons(L.SymbolRef("a"), L.MakeNumber(42)) });
        REQUIRE(L.NumVal(L.Eval(L.SymbolRef("a"), env)) == 42);
    }

    SECTION("A string evaluates to itself") {
        REQUIRE(L.StringVal(L.Eval(L.MakeString("some string"))) == "some string");
    }

    SECTION("Eof evaluates to itself") {
        REQUIRE(L.IsEof(L.Eval(L.Eof)));
    }

    SECTION("A QUOTE form evaluates to its argument, unevaluated") {
        REQUIRE(L.SymbolName(L.Eval(L.Read("(quote foo)"))) == "foo");
        REQUIRE(L.SymbolName(L.Eval(L.Read("'foo"))) == "foo");
    }

    SECTION("SUM of zero arguments is 0") {
        REQUIRE(L.NumVal(L.Eval(L.Read("(+)"))) == 0);
    }

    SECTION("SUM of one or more arguments is the sum of all arguments") {
        REQUIRE(L.NumVal(L.Eval(L.Read("(+ 0)"))) == 0);
        REQUIRE(L.NumVal(L.Eval(L.Read("(+ 2)"))) == 2);
        REQUIRE(L.NumVal(L.Eval(L.Read("(+ 2 3)"))) == 5);
        REQUIRE(L.NumVal(L.Eval(L.Read("(+ 2 3 4)"))) == 9);
    }

    SECTION("SUM works with subexpressions") {
        auto env = L.MakeList({
            L.Cons(L.SymbolRef("a"), L.MakeNumber(1)),
            L.Cons(L.SymbolRef("b"), L.MakeNumber(2)),
            L.Cons(L.SymbolRef("c"), L.MakeNumber(4))
        });
        REQUIRE(L.NumVal(L.Eval(L.Read("(+ (+ a b 8) 16 c)"), env)) == 31);
    }

    SECTION("DIFFERENCE of zero arguments is 0") {
        REQUIRE(L.NumVal(L.Eval(L.Read("(-)"))) == 0);
    }

    SECTION("DIFFERENCE of one argument is the negation of the argument") {
        REQUIRE(L.NumVal(L.Eval(L.Read("(- 0)"))) == 0);
        REQUIRE(L.NumVal(L.Eval(L.Read("(- 2)"))) == -2);
        REQUIRE(L.NumVal(L.Eval(L.Read("(- -2)"))) == 2);
    }

    SECTION("DIFFERENCE of two or more arguments is the first minus the others") {
        REQUIRE(L.NumVal(L.Eval(L.Read("(- 5 2)"))) == 3);
        REQUIRE(L.NumVal(L.Eval(L.Read("(- 5 2 7)"))) == -4);
    }

    SECTION("PRODUCT of zero arguments is 1") {
        REQUIRE(L.NumVal(L.Eval(L.Read("(*)"))) == 1);
    }

    SECTION("PRODUCT of one or more arguments is the product of all arguments") {
        REQUIRE(L.NumVal(L.Eval(L.Read("(* 0)"))) == 0);
        REQUIRE(L.NumVal(L.Eval(L.Read("(* 1)"))) == 1);
        REQUIRE(L.NumVal(L.Eval(L.Read("(* 2)"))) == 2);
        REQUIRE(L.NumVal(L.Eval(L.Read("(* 2 3)"))) == 6);
        REQUIRE(L.NumVal(L.Eval(L.Read("(* 2 3 4)"))) == 24);
    }

    SECTION("QUOTIENT of zero arguments is 1") {
        REQUIRE(L.NumVal(L.Eval(L.Read("(/)"))) == 1);
    }

    SECTION("QUOTIENT of one argument is its reciprocal") {
        REQUIRE(L.NumVal(L.Eval(L.Read("(/ 2)"))) == 0.5);
    }

    SECTION("QUOTIENT of 0 is infinity") {
        REQUIRE(std::isinf(L.NumVal(L.Eval(L.Read("(/ 0)")))));
        REQUIRE(std::isinf(L.NumVal(L.Eval(L.Read("(/ 1 0)")))));
    }

    SECTION("QUOTIENT of two or more arguments is the first divided by the others") {
        REQUIRE(L.NumVal(L.Eval(L.Read("(/ 8 5)"))) == 1.6);
        REQUIRE(L.NumVal(L.Eval(L.Read("(/ 360 4 3)"))) == 30);
    }

    SECTION("A LAMBDA expression evaluates to itself") {
        REQUIRE(L.PrintToString(L.Eval(L.Read("(lambda (n) (+ n 1))"))) == "(lambda (n) (+ n 1))");
    }

    SECTION("LAMBDA call no args") {
        REQUIRE(L.NumVal(L.Eval(L.Read("((lambda () (+ 1 2)))"))) == 3);
    }

    SECTION("LAMBDA call 1 arg") {
        REQUIRE(L.NumVal(L.Eval(L.Read("((lambda (n) (+ n 1)) 1)"))) == 2);
    }

    SECTION("LAMBDA call 2 args") {
        REQUIRE(L.NumVal(L.Eval(L.Read("((lambda (m n) (+ m n 10)) 30 2)"))) == 42);
    }

    SECTION("LAMBDA returns the value of the last expression") {
        REQUIRE(L.Null(L.Eval(L.Read("((lambda ()))"))));
        REQUIRE(L.NumVal(L.Eval(L.Read("((lambda (n) n) 1)"))) == 1);
        REQUIRE(L.NumVal(L.Eval(L.Read("((lambda (n) 1 2 n) 3)"))) == 3);
        REQUIRE(L.NumVal(L.Eval(L.Read("((lambda () 1 2 3 (+ 40 2)))"))) == 42);
    }

    SECTION("PROGN returns the value of the last expression") {
        REQUIRE(L.Null(L.Eval(L.Read("(progn)"))));
        REQUIRE(L.NumVal(L.Eval(L.Read("(progn 1)"))) == 1);
        REQUIRE(L.NumVal(L.Eval(L.Read("(progn 1 2 3)"))) == 3);
        REQUIRE(L.NumVal(L.Eval(L.Read("(progn 1 2 3 (+ 40 2))"))) == 42);
    }

    SECTION("SETQ top level") {
        auto setqReturn = L.Eval(L.Read("(setq a 10)"));
        REQUIRE(L.NumVal(setqReturn) == 10);
        REQUIRE(L.NumVal(L.Eval(L.Read("a"))) == 10);
    }

    SECTION("SETQ in LAMBDA modifies the environment") {
        auto setqReturn = L.Eval(L.Read("(setq a 1)"));
        REQUIRE(L.NumVal(setqReturn) == 1);
        L.Eval(L.Read("(setq f (lambda (a) (progn (setq b a) (setq a 3) (setq c a))))"));
        REQUIRE(L.NumVal(L.Eval(L.Read("(f 2)"))) == 3);
        REQUIRE(L.NumVal(L.Eval(L.Read("a"))) == 1);
        REQUIRE(L.NumVal(L.Eval(L.Read("b"))) == 2);
        REQUIRE(L.NumVal(L.Eval(L.Read("c"))) == 3);
    }

    SECTION("DEF no args") {
        auto defReturn = L.Eval(L.Read("(def f () (+ 1 2))"));
        REQUIRE(L.TypeOf(defReturn) == procdraw::LispObjectType::Cons);
        REQUIRE(L.SymbolName(L.Car(defReturn)) == "lambda");
        REQUIRE(L.NumVal(L.Eval(L.Read("(f)"))) == 3);
    }

    SECTION("DEF 1 arg") {
        auto defReturn = L.Eval(L.Read("(def f (n) (+ n 1))"));
        REQUIRE(L.TypeOf(defReturn) == procdraw::LispObjectType::Cons);
        REQUIRE(L.SymbolName(L.Car(defReturn)) == "lambda");
        REQUIRE(L.NumVal(L.Eval(L.Read("(f 1)"))) == 2);
    }

    SECTION("APPLY no args") {
        REQUIRE(L.NumVal(L.Eval(L.Read("(apply (lambda () (+ 1 2)) (quote ()))"))) == 3);
    }

    SECTION("APPLY 1 arg") {
        REQUIRE(L.NumVal(L.Eval(L.Read("(apply (lambda (n) (+ n 1)) (quote (1)))"))) == 2);
    }

    SECTION("APPLY 2 args") {
        REQUIRE(L.NumVal(L.Eval(L.Read("(apply (lambda (m n) (+ m n 10)) (quote (30 2)))"))) == 42);
    }

    SECTION("CAR") {
        REQUIRE(L.Null(L.Eval(L.Read("(car nil)"))));
        REQUIRE(L.NumVal(L.Eval(L.Read("(car (quote (1 . 2)))"))) == 1);
    }

    SECTION("CDR") {
        REQUIRE(L.Null(L.Eval(L.Read("(cdr nil)"))));
        REQUIRE(L.NumVal(L.Eval(L.Read("(cdr (quote (1 . 2)))"))) == 2);
    }

    SECTION("EQ") {
        REQUIRE(L.BoolVal(L.Eval(L.Read("(eq 42 42)"))));
        REQUIRE_FALSE(L.BoolVal(L.Eval(L.Read("(eq 1 2)"))));
    }

    SECTION("IF") {
        auto env = L.MakeList({
            L.Cons(L.SymbolRef("a"), L.MakeNumber(1)),
            L.Cons(L.SymbolRef("b"), L.MakeNumber(2))
        });

        SECTION("true with else") {
            REQUIRE(L.NumVal(L.Eval(L.Read("(if (eq 42 42) (setq a 10) (setq b 20))"), env)) == 10);
            REQUIRE(L.NumVal(L.Eval(L.Read("a"), env)) == 10);
            REQUIRE(L.NumVal(L.Eval(L.Read("b"), env)) == 2);
        }

        SECTION("true without else") {
            REQUIRE(L.NumVal(L.Eval(L.Read("(if (eq 42 42) (setq a 10))"), env)) == 10);
            REQUIRE(L.NumVal(L.Eval(L.Read("a"), env)) == 10);
            REQUIRE(L.NumVal(L.Eval(L.Read("b"), env)) == 2);
        }

        SECTION("false with else") {
            REQUIRE(L.NumVal(L.Eval(L.Read("(if (eq 1 2) (setq a 10) (setq b 20))"), env)) == 20);
            REQUIRE(L.NumVal(L.Eval(L.Read("a"), env)) == 1);
            REQUIRE(L.NumVal(L.Eval(L.Read("b"), env)) == 20);
        }

        SECTION("false without else") {
            REQUIRE(L.Null(L.Eval(L.Read("(if (eq 1 2) (setq a 10))"), env)));
            REQUIRE(L.NumVal(L.Eval(L.Read("a"), env)) == 1);
            REQUIRE(L.NumVal(L.Eval(L.Read("b"), env)) == 2);
        }
    }

    SECTION("Recursion") {
        auto exp = "(progn"
                   "  (setq f (lambda (n)"
                   "    (if (eq n 0)"
                   "      1"
                   "      (* n (f (- n 1))))))"
                   "  (f 5))";
        REQUIRE(L.NumVal(L.Eval(L.Read(exp))) == 120);
    }

    SECTION("LERP") {

        SECTION("interpolates values for [0, 8]") {
            REQUIRE(L.NumVal(L.Eval(L.Read("(lerp 0 8 0)"))) == 0.0);
            REQUIRE(L.NumVal(L.Eval(L.Read("(lerp 0 8 (/ 4))"))) == 2.0);
            REQUIRE(L.NumVal(L.Eval(L.Read("(lerp 0 8 (/ 2))"))) == 4.0);
            REQUIRE(L.NumVal(L.Eval(L.Read("(lerp 0 8 (/ 3 4))"))) == 6.0);
            REQUIRE(L.NumVal(L.Eval(L.Read("(lerp 0 8 1)"))) == 8.0);
        }

        SECTION("interpolates values for [4, -4]") {
            REQUIRE(L.NumVal(L.Eval(L.Read("(lerp 4 -4 0)"))) == 4.0);
            REQUIRE(L.NumVal(L.Eval(L.Read("(lerp 4 -4 (/ 4))"))) == 2.0);
            REQUIRE(L.NumVal(L.Eval(L.Read("(lerp 4 -4 (/ 2 ))"))) == 0.0);
            REQUIRE(L.NumVal(L.Eval(L.Read("(lerp 4 -4 (/ 3 4))"))) == -2.0);
            REQUIRE(L.NumVal(L.Eval(L.Read("(lerp 4 -4 1)"))) == -4.0);
        }

    }

    SECTION("MAP-RANGE") {

        SECTION("maps values from [0, 10] to [-1, 0]") {
            REQUIRE(L.NumVal(L.Eval(L.Read("(map-range 0 10 -1 0 0)"))) == -1.0);
            REQUIRE(L.NumVal(L.Eval(L.Read("(map-range 0 10 -1 0 (/ 10 4))"))) == -0.75);
            REQUIRE(L.NumVal(L.Eval(L.Read("(map-range 0 10 -1 0 5)"))) == -0.5);
            REQUIRE(L.NumVal(L.Eval(L.Read("(map-range 0 10 -1 0 (/ 30 4))"))) == -0.25);
            REQUIRE(L.NumVal(L.Eval(L.Read("(map-range 0 10 -1 0 10)"))) == 0.0);
        }

        SECTION("maps values from [0, 10] to [1, -1]") {
            REQUIRE(L.NumVal(L.Eval(L.Read("(map-range 0 10 1 -1 0)"))) == 1.0);
            REQUIRE(L.NumVal(L.Eval(L.Read("(map-range 0 10 1 -1 (/ 10 4))"))) == 0.5);
            REQUIRE(L.NumVal(L.Eval(L.Read("(map-range 0 10 1 -1 5)"))) == 0.0);
            REQUIRE(L.NumVal(L.Eval(L.Read("(map-range 0 10 1 -1 (/ 30 4))"))) == -0.5);
            REQUIRE(L.NumVal(L.Eval(L.Read("(map-range 0 10 1 -1 10)"))) == -1.0);
        }

    }

    SECTION("NORM") {

        SECTION("normalizes values for [0, 8]") {
            REQUIRE(L.NumVal(L.Eval(L.Read("(norm 0 8 0)"))) == 0.0);
            REQUIRE(L.NumVal(L.Eval(L.Read("(norm 0 8 2)"))) == 0.25);
            REQUIRE(L.NumVal(L.Eval(L.Read("(norm 0 8 4)"))) == 0.5);
            REQUIRE(L.NumVal(L.Eval(L.Read("(norm 0 8 6)"))) == 0.75);
            REQUIRE(L.NumVal(L.Eval(L.Read("(norm 0 8 8)"))) == 1.0);
        }

        SECTION("normalizes values for [4, -4]") {
            REQUIRE(L.NumVal(L.Eval(L.Read("(norm 4 -4 4)"))) == 0.0);
            REQUIRE(L.NumVal(L.Eval(L.Read("(norm 4 -4 2)"))) == 0.25);
            REQUIRE(L.NumVal(L.Eval(L.Read("(norm 4 -4 0)"))) == 0.5);
            REQUIRE(L.NumVal(L.Eval(L.Read("(norm 4 -4 -2)"))) == 0.75);
            REQUIRE(L.NumVal(L.Eval(L.Read("(norm 4 -4 -4)"))) == 1.0);
        }

    }

    SECTION("WRAP") {

        SECTION("wraps values for [0, 10]") {
            REQUIRE(L.NumVal(L.Eval(L.Read("(wrap 0 10 0)"))) == 0.0);
            REQUIRE(L.NumVal(L.Eval(L.Read("(wrap 0 10 10)"))) == 0.0);
            REQUIRE(L.NumVal(L.Eval(L.Read("(wrap 0 10 20)"))) == 0.0);
            REQUIRE(L.NumVal(L.Eval(L.Read("(wrap 0 10 -10)"))) == 0.0);
            REQUIRE(L.NumVal(L.Eval(L.Read("(wrap 0 10 8)"))) == 8.0);
            REQUIRE(L.NumVal(L.Eval(L.Read("(wrap 0 10 12)"))) == 2.0);
            REQUIRE(L.NumVal(L.Eval(L.Read("(wrap 0 10 23)"))) == 3.0);
            REQUIRE(L.NumVal(L.Eval(L.Read("(wrap 0 10 -2)"))) == 8.0);
            REQUIRE(L.NumVal(L.Eval(L.Read("(wrap 0 10 -13)"))) == 7.0);
        }

        SECTION("wraps values for [-20, -10]") {
            REQUIRE(L.NumVal(L.Eval(L.Read("(wrap -20 -10 -20)"))) == -20.0);
            REQUIRE(L.NumVal(L.Eval(L.Read("(wrap -20 -10 -10)"))) == -20.0);
            REQUIRE(L.NumVal(L.Eval(L.Read("(wrap -20 -10 0)"))) == -20.0);
            REQUIRE(L.NumVal(L.Eval(L.Read("(wrap -20 -10 -30)"))) == -20.0);
            REQUIRE(L.NumVal(L.Eval(L.Read("(wrap -20 -10 -12)"))) == -12.0);
            REQUIRE(L.NumVal(L.Eval(L.Read("(wrap -20 -10 -8)"))) == -18.0);
            REQUIRE(L.NumVal(L.Eval(L.Read("(wrap -20 -10 13)"))) == -17.0);
            REQUIRE(L.NumVal(L.Eval(L.Read("(wrap -20 -10 -22)"))) == -12.0);
            REQUIRE(L.NumVal(L.Eval(L.Read("(wrap -20 -10 -33)"))) == -13.0);
        }

    }

    SECTION("ASSOC returns nil if key is not found") {
        REQUIRE(L.Null(L.Eval(L.Read("(assoc 'a '())"))));
        REQUIRE(L.Null(L.Eval(L.Read("(assoc 'a '((b . 1)))"))));
        REQUIRE(L.Null(L.Eval(L.Read("(assoc 'a '((b . 1) (c . 2)))"))));
    }

    SECTION("ASSOC returns the first match") {
        REQUIRE(L.PrintToString(L.Eval(L.Read("(assoc 'a '((a . 1)))"))) == "(a . 1)");
        REQUIRE(L.PrintToString(L.Eval(L.Read("(assoc 'a '((b . 1) (a . 2)))"))) == "(a . 2)");
        REQUIRE(L.PrintToString(L.Eval(L.Read("(assoc 'a '((b . 1) (a . 2) (a . 3)))"))) == "(a . 2)");
    }

    SECTION("PUTASSOC updates the first match, if one is found, and returns val") {
        L.Eval(L.Read("(setq alist '((a . 1)))"));
        REQUIRE(L.NumVal(L.Eval(L.Read("(putassoc 'a 2 alist)"))) == 2);
        REQUIRE(L.PrintToString(L.Eval(L.Read("alist"))) == "((a . 2))");
        L.Eval(L.Read("(setq alist '((b . 1) (a . 2) (a . 3)))"));
        REQUIRE(L.NumVal(L.Eval(L.Read("(putassoc 'a 4 alist)"))) == 4);
        REQUIRE(L.PrintToString(L.Eval(L.Read("alist"))) == "((b . 1) (a . 4) (a . 3))");
    }

    SECTION("PUTASSOC adds onto the end if no match is found and returns val") {
        L.Eval(L.Read("(setq alist '((b . 1)))"));
        REQUIRE(L.NumVal(L.Eval(L.Read("(putassoc 'a 2 alist)"))) == 2);
        REQUIRE(L.PrintToString(L.Eval(L.Read("alist"))) == "((b . 1) (a . 2))");
        REQUIRE(L.NumVal(L.Eval(L.Read("(putassoc 'c 3 alist)"))) == 3);
        REQUIRE(L.PrintToString(L.Eval(L.Read("alist"))) == "((b . 1) (a . 2) (c . 3))");
    }

    SECTION("NOT negates the boolean value of an expression") {
        REQUIRE(L.BoolVal(L.Eval(L.Read("(not true)"))) == false);
        REQUIRE(L.BoolVal(L.Eval(L.Read("(not false)"))) == true);
        REQUIRE(L.BoolVal(L.Eval(L.Read("(not nil)"))) == true);
        REQUIRE(L.BoolVal(L.Eval(L.Read("(not 0)"))) == false);
        REQUIRE(L.BoolVal(L.Eval(L.Read("(not 1)"))) == false);
        REQUIRE(L.BoolVal(L.Eval(L.Read("(not 2)"))) == false);
        REQUIRE(L.BoolVal(L.Eval(L.Read("(not 'hello)"))) == false);
    }

}

static int testCfunData = 42;

static procdraw::LispObjectPtr TestCfun(procdraw::LispInterpreter *L,
                                        procdraw::LispObjectPtr args,
                                        procdraw::LispObjectPtr env,
                                        void *data)
{
    REQUIRE(data == &testCfunData);
    int dataval = *(static_cast<int*>(data));
    REQUIRE(dataval == 42);
    return L->MakeNumber(L->NumVal(L->Car(args)) + dataval);
}

TEST_CASE("LispInterpreter::SetGlobalCFunction")
{
    procdraw::LispInterpreter L;

    SECTION("test-cfun should return the sum of its first argument and testCfunData") {
        L.SetGlobalCFunction("test-cfun", TestCfun, &testCfunData);
        REQUIRE(L.NumVal(L.Eval(L.Read("(test-cfun 10)"))) == 52);
    }
}
