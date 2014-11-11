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

    SECTION("Multiple reads") {
        auto a = reader.Read(&L, "100");
        REQUIRE(L.NumVal(a) == 100);
        auto b = reader.Read(&L, "20");
        REQUIRE(L.NumVal(b) == 20);
        auto c = reader.Read(&L, "300");
        REQUIRE(L.NumVal(c) == 300);
    }

    SECTION("Symbol") {
        auto obj = reader.Read(&L, "HELLO-WORLD-1");
        REQUIRE(L.TypeOf(obj) == procdraw::LispObjectType::Symbol);
        REQUIRE(L.StringVal(obj) == "HELLO-WORLD-1");
    }

    SECTION("Nil") {
        auto obj = reader.Read(&L, "nil");
        REQUIRE(L.Null(obj));
    }

    SECTION("Empty list") {
        auto obj = reader.Read(&L, "()");
        REQUIRE(L.Null(obj));
    }

    SECTION("List with single item") {
        auto obj = reader.Read(&L, "(42)");
        REQUIRE(L.TypeOf(obj) == procdraw::LispObjectType::Cons);
        REQUIRE(L.NumVal(L.Car(obj)) == 42);
        REQUIRE(L.Null(L.Cdr(obj)));
    }

    // +---+  +---+  +---+  +---+
    // |1|*|->|2|*|->|3|*|->|4|/|
    // +---+  +---+  +---+  +---+
    //
    SECTION("List with multiple items") {
        auto list1 = reader.Read(&L, "(1 2 3 4)");
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
    SECTION("List with embedded list") {
        auto list1 = reader.Read(&L, "((1 2) 3 4)");
        REQUIRE(L.TypeOf(list1) == procdraw::LispObjectType::Cons);
        REQUIRE(L.NumVal(L.Car(L.Car(list1))) == 1);
        REQUIRE(L.NumVal(L.Car(L.Cdr(L.Car(list1)))) == 2);
        REQUIRE(L.Null(L.Cdr(L.Cdr(L.Car(list1)))));
        REQUIRE(L.NumVal(L.Car(L.Cdr(list1))) == 3);
        REQUIRE(L.NumVal(L.Car(L.Cdr(L.Cdr(list1)))) == 4);
        REQUIRE(L.Null(L.Cdr(L.Cdr(L.Cdr(list1)))));
    }

    SECTION("Dotted pair") {
        auto obj = reader.Read(&L, "(2 . 3)");
        REQUIRE(L.NumVal(L.Car(obj)) == 2);
        REQUIRE(L.NumVal(L.Cdr(obj)) == 3);
    }

}
