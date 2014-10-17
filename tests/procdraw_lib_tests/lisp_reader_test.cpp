#include "lisp_reader.h"
#include "catch.hpp"

TEST_CASE("LispReader") {

    procdraw::LispMemory mem;
    procdraw::LispReader reader(mem);

    SECTION("Integer") {
        auto obj = reader.Read("42");
        REQUIRE(mem.TypeOf(obj) == procdraw::LispObjectType::Number);
        REQUIRE(mem.NumVal(obj) == 42);
    }

    SECTION("Symbol") {
        auto obj = reader.Read("HELLO-WORLD-1");
        REQUIRE(mem.TypeOf(obj) == procdraw::LispObjectType::Symbol);
        REQUIRE(mem.StringVal(obj) == "HELLO-WORLD-1");
    }

    SECTION("Empty list") {
        auto obj = reader.Read("()");
        REQUIRE(mem.Null(obj));
    }

    SECTION("List with single element") {
        auto obj = reader.Read("(42)");
        REQUIRE(mem.TypeOf(obj) == procdraw::LispObjectType::Cons);
        REQUIRE(mem.NumVal(mem.Car(obj)) == 42);
        REQUIRE(mem.Null(mem.Cdr(obj)));
    }

    // A      B      C
    // +---+  +---+  +---+
    // |2|*|->|3|*|->|5|/|
    // +---+  +---+  +---+
    SECTION("List with multiple elements") {
        auto consA = reader.Read("(2 3 5)");
        REQUIRE(mem.NumVal(mem.Car(consA)) == 2);
        auto consB = mem.Cdr(consA);
        REQUIRE(mem.NumVal(mem.Car(consB)) == 3);
        auto consC = mem.Cdr(consB);
        REQUIRE(mem.NumVal(mem.Car(consC)) == 5);
        REQUIRE(mem.Null(mem.Cdr(consC)));
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
        auto consA = reader.Read("(2 (3 5) 7)");
        REQUIRE(mem.NumVal(mem.Car(consA)) == 2);
        auto consB = mem.Cdr(consA);
        auto consC = mem.Cdr(consB);
        REQUIRE(mem.NumVal(mem.Car(consC)) == 7);
        REQUIRE(mem.Null(mem.Cdr(consC)));
        auto consD = mem.Car(consB);
        REQUIRE(mem.NumVal(mem.Car(consD)) == 3);
        auto consE = mem.Cdr(consD);
        REQUIRE(mem.NumVal(mem.Car(consE)) == 5);
        REQUIRE(mem.Null(mem.Cdr(consE)));
    }

    SECTION("Dotted pair") {
        auto obj = reader.Read("(2 . 3)");
        REQUIRE(mem.NumVal(mem.Car(obj)) == 2);
        REQUIRE(mem.NumVal(mem.Cdr(obj)) == 3);
    }

}
