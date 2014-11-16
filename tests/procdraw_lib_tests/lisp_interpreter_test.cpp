#include "lisp_interpreter.h"
#include "catch.hpp"

TEST_CASE("LispInterpreter::List()") {

    procdraw::LispInterpreter L;

    SECTION("Empty list") {
        REQUIRE(L.Null(L.List({})));
    }

    SECTION("Single item") {
        auto list1 = L.List({ L.MakeNumber(42) });
        REQUIRE(L.TypeOf(list1) == procdraw::LispObjectType::Cons);
        REQUIRE(L.NumVal(L.Car(list1)) == 42);
        REQUIRE(L.Null(L.Cdr(list1)));
    }

    // +---+  +---+  +---+  +---+
    // |1|*|->|2|*|->|3|*|->|4|/|
    // +---+  +---+  +---+  +---+
    //
    SECTION("Multiple items") {
        auto list1 = L.List({ L.MakeNumber(1), L.MakeNumber(2), L.MakeNumber(3), L.MakeNumber(4) });
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
        auto list1 = L.List({ L.List({ L.MakeNumber(1), L.MakeNumber(2) }), L.MakeNumber(3), L.MakeNumber(4) });
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
        REQUIRE(L.PrintString(L.List({})) == "nil");
        REQUIRE(L.PrintString(L.List({ L.MakeNumber(42) })) == "(42)");
        REQUIRE(L.PrintString(L.List({ L.MakeNumber(1), L.MakeNumber(2), L.MakeNumber(3), L.MakeNumber(4) })) == "(1 2 3 4)");
        REQUIRE(L.PrintString(L.List({ L.List({ L.MakeNumber(1), L.MakeNumber(2) }), L.MakeNumber(3), L.MakeNumber(4) })) == "((1 2) 3 4)");
    }

    SECTION("Dotted pair") {
        REQUIRE(L.PrintString(L.Cons(L.MakeNumber(1), L.MakeNumber(2))) == "(1 . 2)");
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

    auto list1 = L.List({ L.MakeNumber(42) });
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
        auto alist = L.List({ L.Cons(symbolA, L.MakeNumber(1)), L.Cons(symbolB, L.MakeNumber(2)) });

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

TEST_CASE("LispInterpreter::Eval()") {

    procdraw::LispInterpreter L;

    SECTION("Number") {
        auto result = L.Eval(L.MakeNumber(42), L.Nil);
        REQUIRE(L.NumVal(result) == 42);
    }

    SECTION("Retrieve undefined") {
        REQUIRE(L.Null(L.Eval(L.SymbolRef("a"), L.Nil)));
    }

    SECTION("Retrieve number") {
        auto env = L.List({ L.Cons(L.SymbolRef("a"), L.MakeNumber(42)) });
        auto result = L.Eval(L.SymbolRef("a"), env);
        REQUIRE(L.NumVal(result) == 42);
    }

    SECTION("QUOTE") {
        auto result = L.Eval(L.Read("(quote 42)"), L.Nil);
        REQUIRE(L.NumVal(result) == 42);
    }

    SECTION("ADD") {
        auto env = L.List({
            L.Cons(L.SymbolRef("a"), L.MakeNumber(1)),
            L.Cons(L.SymbolRef("b"), L.MakeNumber(2)),
            L.Cons(L.SymbolRef("c"), L.MakeNumber(4))
        });
        auto result = L.Eval(L.Read("(add (add a b 8) 16 c)"), env);
        REQUIRE(L.NumVal(result) == 31);
    }

    SECTION("LAMBDA expression evaluates to itself") {
        auto result = L.Eval(L.Read("(lambda (n) (add n 1))"));
        REQUIRE(L.PrintString(result) == "(lambda (n) (add n 1))");
    }

    SECTION("LAMBDA call no args") {
        auto result = L.Eval(L.Read("((lambda () (add 1 2)))"));
        REQUIRE(L.NumVal(result) == 3);
    }

    SECTION("LAMBDA call 1 arg") {
        auto result = L.Eval(L.Read("((lambda (n) (add n 1)) 1)"));
        REQUIRE(L.NumVal(result) == 2);
    }

    SECTION("LAMBDA call 2 args") {
        auto result = L.Eval(L.Read("((lambda (m n) (add m n 10)) 30 2)"));
        REQUIRE(L.NumVal(result) == 42);
    }

    SECTION("PROGN") {
        REQUIRE(L.Null(L.Eval(L.Read("(progn)"))));
        REQUIRE(L.NumVal(L.Eval(L.Read("(progn 1)"))) == 1);
        REQUIRE(L.NumVal(L.Eval(L.Read("(progn 1 2 3)"))) == 3);
        REQUIRE(L.NumVal(L.Eval(L.Read("(progn 1 2 3 (add 40 2))"))) == 42);
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
        auto result = L.Eval(L.Read("(apply (lambda () (add 1 2)) (quote ()))"));
        REQUIRE(L.NumVal(result) == 3);
    }

    SECTION("APPLY 1 arg") {
        auto result = L.Eval(L.Read("(apply (lambda (n) (add n 1)) (quote (1)))"));
        REQUIRE(L.NumVal(result) == 2);
    }

    SECTION("APPLY 2 args") {
        auto result = L.Eval(L.Read("(apply (lambda (m n) (add m n 10)) (quote (30 2)))"));
        REQUIRE(L.NumVal(result) == 42);
    }

}
