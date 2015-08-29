#include "lisp_printer.h"
#include "lisp_interpreter.h"
#include "catch.hpp"

TEST_CASE("Lisp printer")
{

    procdraw::LispPrinter printer;
    procdraw::LispInterpreter L;

    SECTION("A positive integer is printed without sign") {
        REQUIRE(printer.PrintToString(&L, L.MakeNumber(42)) == "42");
    }

    SECTION("A negative integer is printed with sign") {
        REQUIRE(printer.PrintToString(&L, L.MakeNumber(-42)) == "-42");
    }

    SECTION("A symbol is printed as its name") {
        REQUIRE(printer.PrintToString(&L, L.SymbolRef("HELLO")) == "HELLO");
    }

    SECTION("The empty list is printed as nil") {
        REQUIRE(printer.PrintToString(&L, L.MakeList({})) == "nil");
    }

    SECTION("Lists are printed with parens to mark start and end and with space to separate elements") {
        REQUIRE(printer.PrintToString(&L, L.MakeList({ L.MakeNumber(42) })) == "(42)");
        REQUIRE(printer.PrintToString(&L, L.MakeList({ L.MakeNumber(1), L.MakeNumber(2), L.MakeNumber(3), L.MakeNumber(4) })) ==
                "(1 2 3 4)");
        REQUIRE(printer.PrintToString(&L, L.MakeList({ L.MakeList({ L.MakeNumber(1), L.MakeNumber(2) }), L.MakeNumber(3), L.MakeNumber(4) }))
                == "((1 2) 3 4)");
    }

    SECTION("Cons cells with a non-nil atomic cdr value are printed as dotted pair") {
        REQUIRE(printer.PrintToString(&L, L.Cons(L.MakeNumber(1), L.MakeNumber(2))) == "(1 . 2)");
        REQUIRE(printer.PrintToString(&L, L.Cons(L.MakeNumber(2), L.Cons(L.MakeNumber(3), L.MakeNumber(4)))) == "(2 3 . 4)");
    }

    SECTION("Booleans are printed as true and false") {
        REQUIRE(printer.PrintToString(&L, L.True) == "true");
        REQUIRE(printer.PrintToString(&L, L.False) == "false");
    }

    SECTION("A string is printed using double quotes") {
        REQUIRE(printer.PrintToString(&L, L.MakeString("some string")) == "\"some string\"");
    }

    SECTION("Tables TODO") {
        REQUIRE(printer.PrintToString(&L, L.MakeTable()) == "<Table>");
    }

    SECTION("A quote form is printed using prefixed single quote") {
        REQUIRE(printer.PrintToString(&L, L.MakeList({ L.SymbolRef("quote"), L.MakeNumber(42) })) == "'42");
    }

}
