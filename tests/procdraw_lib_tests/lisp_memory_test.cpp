#include "lisp_memory.h"
#include "lisp_reader.h"
#include "catch.hpp"

TEST_CASE("LispMemory.MakeSymbol()") {

    procdraw::LispMemory mem;

    auto hello1 = mem.MakeSymbol("HELLO");
    REQUIRE(mem.TypeOf(hello1) == procdraw::LispObjectType::Symbol);
    REQUIRE(mem.StringVal(hello1) == "HELLO");
    auto hello2 = mem.MakeSymbol("HELLO");
    REQUIRE(mem.TypeOf(hello2) == procdraw::LispObjectType::Symbol);
    REQUIRE(mem.StringVal(hello2) == "HELLO");
    // Verify that they are the same address
    REQUIRE(hello1.get() == hello2.get());
}
