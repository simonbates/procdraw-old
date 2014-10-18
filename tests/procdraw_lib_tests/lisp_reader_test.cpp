#include "lisp_reader.h"
#include "lisp_interpreter.h"
#include "catch.hpp"

TEST_CASE("LispReader") {

    procdraw::LispReader reader;
    procdraw::LispInterpreter L;

    SECTION("Integer") {
        auto obj = reader.Read(&L, "42");
        REQUIRE(L.TypeOf(obj) == procdraw::LispObjectType::Number);
        REQUIRE(L.NumVal(obj) == 42);
    }

    SECTION("Symbol") {
        auto obj = reader.Read(&L, "HELLO-WORLD-1");
        REQUIRE(L.TypeOf(obj) == procdraw::LispObjectType::Symbol);
        REQUIRE(L.StringVal(obj) == "HELLO-WORLD-1");
    }

    SECTION("Empty list") {
        auto obj = reader.Read(&L, "()");
        REQUIRE(L.Null(obj));
    }

    SECTION("List with single element") {
        auto obj = reader.Read(&L, "(42)");
        REQUIRE(L.TypeOf(obj) == procdraw::LispObjectType::Cons);
        REQUIRE(L.NumVal(L.Car(obj)) == 42);
        REQUIRE(L.Null(L.Cdr(obj)));
    }

    // A      B      C
    // +---+  +---+  +---+
    // |2|*|->|3|*|->|5|/|
    // +---+  +---+  +---+
    SECTION("List with multiple elements") {
        auto consA = reader.Read(&L, "(2 3 5)");
        REQUIRE(L.NumVal(L.Car(consA)) == 2);
        auto consB = L.Cdr(consA);
        REQUIRE(L.NumVal(L.Car(consB)) == 3);
        auto consC = L.Cdr(consB);
        REQUIRE(L.NumVal(L.Car(consC)) == 5);
        REQUIRE(L.Null(L.Cdr(consC)));
    }

    // A      B      C
    // +---+  +---+  +---+
    // |2|*|->|*|*|->|7|/|
    // +---+  +---+  +---+
    //         |
    //         v
    //        +---+  +---+
    //        |3|*|->|5|/|
    //        +---+  +---+
    //        D      E
    SECTION("List with embedded list") {
        auto consA = reader.Read(&L, "(2 (3 5) 7)");
        REQUIRE(L.NumVal(L.Car(consA)) == 2);
        auto consB = L.Cdr(consA);
        auto consC = L.Cdr(consB);
        REQUIRE(L.NumVal(L.Car(consC)) == 7);
        REQUIRE(L.Null(L.Cdr(consC)));
        auto consD = L.Car(consB);
        REQUIRE(L.NumVal(L.Car(consD)) == 3);
        auto consE = L.Cdr(consD);
        REQUIRE(L.NumVal(L.Car(consE)) == 5);
        REQUIRE(L.Null(L.Cdr(consE)));
    }

    SECTION("Dotted pair") {
        auto obj = reader.Read(&L, "(2 . 3)");
        REQUIRE(L.NumVal(L.Car(obj)) == 2);
        REQUIRE(L.NumVal(L.Cdr(obj)) == 3);
    }

}
