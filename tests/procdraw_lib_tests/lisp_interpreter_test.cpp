#include "lisp_interpreter.h"
#include "catch.hpp"

TEST_CASE("LispInterpreter.PrintString()") {

    procdraw::LispInterpreter interp;

    SECTION("Integer") {
        REQUIRE(interp.PrintString(interp.Read("42")) == "42");
    }

    SECTION("Symbol") {
        REQUIRE(interp.PrintString(interp.Read("HELLO")) == "HELLO");
    }

    SECTION("List") {
        REQUIRE(interp.PrintString(interp.Read("()")) == "NIL");
        REQUIRE(interp.PrintString(interp.Read("(42)")) == "(42)");
        REQUIRE(interp.PrintString(interp.Read("(2 3 5)")) == "(2 3 5)");
        REQUIRE(interp.PrintString(interp.Read("(2 (3 5) 7)")) == "(2 (3 5) 7)");
    }

    SECTION("Dotted pair") {
        REQUIRE(interp.PrintString(interp.Read("(2 . 3)")) == "(2 . 3)");
    }

}
