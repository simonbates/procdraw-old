#include "lisp_interpreter.h"
#include "catch.hpp"

TEST_CASE("LispInterpreter::PrintString()") {

    procdraw::LispInterpreter L;

    SECTION("Integer") {
        REQUIRE(L.PrintString(L.Read("42")) == "42");
    }

    SECTION("Symbol") {
        REQUIRE(L.PrintString(L.Read("HELLO")) == "HELLO");
    }

    SECTION("List") {
        REQUIRE(L.PrintString(L.Read("()")) == "NIL");
        REQUIRE(L.PrintString(L.Read("(42)")) == "(42)");
        REQUIRE(L.PrintString(L.Read("(2 3 5)")) == "(2 3 5)");
        REQUIRE(L.PrintString(L.Read("(2 (3 5) 7)")) == "(2 (3 5) 7)");
    }

    SECTION("Dotted pair") {
        REQUIRE(L.PrintString(L.Read("(2 . 3)")) == "(2 . 3)");
    }

}

TEST_CASE("LispInterpreter::MakeSymbol()") {

    procdraw::LispInterpreter L;

    auto hello1 = L.MakeSymbol("HELLO");
    REQUIRE(L.TypeOf(hello1) == procdraw::LispObjectType::Symbol);
    REQUIRE(L.StringVal(hello1) == "HELLO");
    auto hello2 = L.MakeSymbol("HELLO");
    REQUIRE(L.TypeOf(hello2) == procdraw::LispObjectType::Symbol);
    REQUIRE(L.StringVal(hello2) == "HELLO");
    // Verify that they are the same address
    REQUIRE(hello1.get() == hello2.get());
}
