#include "lisp_interpreter.h"
#include "catch.hpp"
#include <cmath>

TEST_CASE("LispInterpreter::MakeList()") {

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

TEST_CASE("LispInterpreter::PrintString()") {

    procdraw::LispInterpreter L;

    SECTION("Integer") {
        REQUIRE(L.PrintString(L.MakeNumber(42)) == "42");
    }

    SECTION("Symbol") {
        REQUIRE(L.PrintString(L.SymbolRef("HELLO")) == "HELLO");
    }

    SECTION("List") {
        REQUIRE(L.PrintString(L.MakeList({})) == "nil");
        REQUIRE(L.PrintString(L.MakeList({ L.MakeNumber(42) })) == "(42)");
        REQUIRE(L.PrintString(L.MakeList({ L.MakeNumber(1), L.MakeNumber(2), L.MakeNumber(3), L.MakeNumber(4) })) == "(1 2 3 4)");
        REQUIRE(L.PrintString(L.MakeList({ L.MakeList({ L.MakeNumber(1), L.MakeNumber(2) }), L.MakeNumber(3), L.MakeNumber(4) })) == "((1 2) 3 4)");
    }

    SECTION("Dotted pair") {
        REQUIRE(L.PrintString(L.Cons(L.MakeNumber(1), L.MakeNumber(2))) == "(1 . 2)");
    }

    SECTION("Boolean") {
        REQUIRE(L.PrintString(L.True) == "true");
        REQUIRE(L.PrintString(L.False) == "false");
    }

}

TEST_CASE("LispInterpreter::SymbolRef()") {

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

TEST_CASE("LispInterpreter::Eq()") {

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

TEST_CASE("LispInterpreter::Assoc()") {

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

TEST_CASE("LispInterpreter::Rplaca()") {

    procdraw::LispInterpreter L;

    auto cons = L.Cons(L.MakeNumber(1), L.MakeNumber(2));
    auto result = L.Rplaca(cons, L.MakeNumber(10));
    REQUIRE(L.NumVal(L.Car(cons)) == 10);
    REQUIRE(L.NumVal(L.Cdr(cons)) == 2);
    REQUIRE(L.Eq(cons, result));
}

TEST_CASE("LispInterpreter::Rplacd()") {

    procdraw::LispInterpreter L;

    auto cons = L.Cons(L.MakeNumber(1), L.MakeNumber(2));
    auto result = L.Rplacd(cons, L.MakeNumber(20));
    REQUIRE(L.NumVal(L.Car(cons)) == 1);
    REQUIRE(L.NumVal(L.Cdr(cons)) == 20);
    REQUIRE(L.Eq(cons, result));
}

TEST_CASE("LispInterpreter implicit type conversion") {

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
            REQUIRE(std::isnan(L.NumVal(L.SetGlobalCFunction("cfunction", nullptr))));
            // Boolean
            REQUIRE(std::isnan(L.NumVal(L.True)));
            REQUIRE(std::isnan(L.NumVal(L.False)));
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
            REQUIRE(L.BoolVal(L.SetGlobalCFunction("cfunction", nullptr)));
            // Boolean
            REQUIRE(L.BoolVal(L.True));
            REQUIRE_FALSE(L.BoolVal(L.False));
        }
    }

}

TEST_CASE("LispInterpreter::Eval()") {

    procdraw::LispInterpreter L;

    SECTION("Nil") {
        REQUIRE(L.Null(L.Eval(L.Nil)));
    }

    SECTION("Number") {
        auto result = L.Eval(L.MakeNumber(42));
        REQUIRE(L.NumVal(result) == 42);
    }

    SECTION("Booleans") {
        REQUIRE(L.BoolVal(L.Eval(L.True)));
        REQUIRE_FALSE(L.BoolVal(L.Eval(L.False)));
    }

    SECTION("Retrieve undefined") {
        REQUIRE(L.Null(L.Eval(L.SymbolRef("a"))));
    }

    SECTION("Retrieve number") {
        auto env = L.MakeList({ L.Cons(L.SymbolRef("a"), L.MakeNumber(42)) });
        auto result = L.Eval(L.SymbolRef("a"), env);
        REQUIRE(L.NumVal(result) == 42);
    }

    SECTION("QUOTE") {
        auto result = L.Eval(L.Read("(quote 42)"));
        REQUIRE(L.NumVal(result) == 42);
    }

    SECTION("SUM") {
        REQUIRE(L.NumVal(L.Eval(L.Read("(+)"))) == 0);
        REQUIRE(L.NumVal(L.Eval(L.Read("(+ 0)"))) == 0);
        REQUIRE(L.NumVal(L.Eval(L.Read("(+ 2)"))) == 2);
        REQUIRE(L.NumVal(L.Eval(L.Read("(+ 2 3)"))) == 5);
        REQUIRE(L.NumVal(L.Eval(L.Read("(+ 2 3 4)"))) == 9);
    }

    SECTION("SUM with subexpressions") {
        auto env = L.MakeList({
            L.Cons(L.SymbolRef("a"), L.MakeNumber(1)),
            L.Cons(L.SymbolRef("b"), L.MakeNumber(2)),
            L.Cons(L.SymbolRef("c"), L.MakeNumber(4))
        });
        auto result = L.Eval(L.Read("(+ (+ a b 8) 16 c)"), env);
        REQUIRE(L.NumVal(result) == 31);
    }

    SECTION("DIFFERENCE") {
        REQUIRE(L.NumVal(L.Eval(L.Read("(-)"))) == 0);
        REQUIRE(L.NumVal(L.Eval(L.Read("(- 0)"))) == 0);
        REQUIRE(L.NumVal(L.Eval(L.Read("(- 2)"))) == -2);
        REQUIRE(L.NumVal(L.Eval(L.Read("(- 5 2)"))) == 3);
        REQUIRE(L.NumVal(L.Eval(L.Read("(- 5 2 7)"))) == -4);
    }

    SECTION("PRODUCT") {
        REQUIRE(L.NumVal(L.Eval(L.Read("(*)"))) == 1);
        REQUIRE(L.NumVal(L.Eval(L.Read("(* 0)"))) == 0);
        REQUIRE(L.NumVal(L.Eval(L.Read("(* 1)"))) == 1);
        REQUIRE(L.NumVal(L.Eval(L.Read("(* 2)"))) == 2);
        REQUIRE(L.NumVal(L.Eval(L.Read("(* 2 3)"))) == 6);
        REQUIRE(L.NumVal(L.Eval(L.Read("(* 2 3 4)"))) == 24);
    }

    SECTION("QUOTIENT") {
        REQUIRE(L.NumVal(L.Eval(L.Read("(/)"))) == 1);
        REQUIRE(std::isinf(L.NumVal(L.Eval(L.Read("(/ 0)")))));
        REQUIRE(std::isinf(L.NumVal(L.Eval(L.Read("(/ 1 0)")))));
        REQUIRE(L.NumVal(L.Eval(L.Read("(/ 2)"))) == 0.5);
        REQUIRE(L.NumVal(L.Eval(L.Read("(/ 8 5)"))) == 1.6);
        REQUIRE(L.NumVal(L.Eval(L.Read("(/ 360 4 3)"))) == 30);
    }

    SECTION("LAMBDA expression evaluates to itself") {
        auto result = L.Eval(L.Read("(lambda (n) (+ n 1))"));
        REQUIRE(L.PrintString(result) == "(lambda (n) (+ n 1))");
    }

    SECTION("LAMBDA call no args") {
        auto result = L.Eval(L.Read("((lambda () (+ 1 2)))"));
        REQUIRE(L.NumVal(result) == 3);
    }

    SECTION("LAMBDA call 1 arg") {
        auto result = L.Eval(L.Read("((lambda (n) (+ n 1)) 1)"));
        REQUIRE(L.NumVal(result) == 2);
    }

    SECTION("LAMBDA call 2 args") {
        auto result = L.Eval(L.Read("((lambda (m n) (+ m n 10)) 30 2)"));
        REQUIRE(L.NumVal(result) == 42);
    }

    SECTION("PROGN") {
        REQUIRE(L.Null(L.Eval(L.Read("(progn)"))));
        REQUIRE(L.NumVal(L.Eval(L.Read("(progn 1)"))) == 1);
        REQUIRE(L.NumVal(L.Eval(L.Read("(progn 1 2 3)"))) == 3);
        REQUIRE(L.NumVal(L.Eval(L.Read("(progn 1 2 3 (+ 40 2))"))) == 42);
    }

    SECTION("SETQ top level") {
        auto setqReturn = L.Eval(L.Read("(setq a 10)"));
        REQUIRE(L.NumVal(setqReturn) == 10);
        auto aVal = L.Eval(L.Read("a"));
        REQUIRE(L.NumVal(aVal) == 10);
    }

    SECTION("SETQ in LAMBDA") {
        auto setqReturn = L.Eval(L.Read("(setq a 1)"));
        REQUIRE(L.NumVal(setqReturn) == 1);
        L.Eval(L.Read("(setq f (lambda (a) (progn (setq b a) (setq a 3) (setq c a))))"));
        auto fReturn = L.Eval(L.Read("(f 2)"));
        REQUIRE(L.NumVal(fReturn) == 3);
        auto aVal = L.Eval(L.Read("a"));
        REQUIRE(L.NumVal(aVal) == 1);
        auto bVal = L.Eval(L.Read("b"));
        REQUIRE(L.NumVal(bVal) == 2);
        auto cVal = L.Eval(L.Read("c"));
        REQUIRE(L.NumVal(cVal) == 3);
    }

    SECTION("APPLY no args") {
        auto result = L.Eval(L.Read("(apply (lambda () (+ 1 2)) (quote ()))"));
        REQUIRE(L.NumVal(result) == 3);
    }

    SECTION("APPLY 1 arg") {
        auto result = L.Eval(L.Read("(apply (lambda (n) (+ n 1)) (quote (1)))"));
        REQUIRE(L.NumVal(result) == 2);
    }

    SECTION("APPLY 2 args") {
        auto result = L.Eval(L.Read("(apply (lambda (m n) (+ m n 10)) (quote (30 2)))"));
        REQUIRE(L.NumVal(result) == 42);
    }

}
