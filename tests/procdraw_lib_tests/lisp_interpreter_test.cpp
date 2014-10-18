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
        REQUIRE(L.PrintString(L.List({})) == "NIL");
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
    REQUIRE(L.StringVal(hello1) == "HELLO");
    auto hello2 = L.SymbolRef("HELLO");
    REQUIRE(L.TypeOf(hello2) == procdraw::LispObjectType::Symbol);
    REQUIRE(L.StringVal(hello2) == "HELLO");
    // Verify that they are the same address
    REQUIRE(hello1.get() == hello2.get());
}

TEST_CASE("LispInterpreter::Eq()") {

    procdraw::LispInterpreter L;

    REQUIRE(L.Eq(L.Nil, L.Nil));

    REQUIRE(L.Eq(L.MakeNumber(42), L.MakeNumber(42)));
    REQUIRE_FALSE(L.Eq(L.MakeNumber(42), L.MakeNumber(1)));

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
