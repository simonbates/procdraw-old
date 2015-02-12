#include "line_buffer.h"
#include "catch.hpp"

TEST_CASE("LineBuffer void constructor")
{
    procdraw::LineBuffer b;
    REQUIRE(b.GetLine() == std::wstring(L""));
    REQUIRE(b.GetUserText() == std::wstring(L""));
    b.Insert('a');
    REQUIRE(b.GetLine() == std::wstring(L"a"));
    REQUIRE(b.GetUserText() == std::wstring(L"a"));
}

TEST_CASE("LineBuffer empty prompt")
{
    procdraw::LineBuffer b(L"");
    REQUIRE(b.GetLine() == std::wstring(L""));
    REQUIRE(b.GetUserText() == std::wstring(L""));
}

TEST_CASE("LineBuffer non empty prompt")
{
    procdraw::LineBuffer b(L"> ");
    REQUIRE(b.GetLine() == std::wstring(L"> "));
    REQUIRE(b.GetUserText() == std::wstring(L""));
}

TEST_CASE("LineBuffer insert and delete")
{
    procdraw::LineBuffer b(L"> ");
    REQUIRE(b.GetLine() == std::wstring(L"> "));
    REQUIRE(b.GetUserText() == std::wstring(L""));
    b.Insert('a');
    REQUIRE(b.GetLine() == std::wstring(L"> a"));
    REQUIRE(b.GetUserText() == std::wstring(L"a"));
    b.Insert('b');
    REQUIRE(b.GetLine() == std::wstring(L"> ab"));
    REQUIRE(b.GetUserText() == std::wstring(L"ab"));
    b.DeleteBack();
    REQUIRE(b.GetLine() == std::wstring(L"> a"));
    REQUIRE(b.GetUserText() == std::wstring(L"a"));
    b.DeleteBack();
    REQUIRE(b.GetLine() == std::wstring(L"> "));
    REQUIRE(b.GetUserText() == std::wstring(L""));
}

TEST_CASE("LineBuffer cannot delete prompt")
{
    procdraw::LineBuffer b(L"> ");
    REQUIRE(b.GetLine() == std::wstring(L"> "));
    REQUIRE(b.GetUserText() == std::wstring(L""));
    b.Insert('a');
    REQUIRE(b.GetLine() == std::wstring(L"> a"));
    REQUIRE(b.GetUserText() == std::wstring(L"a"));
    b.DeleteBack();
    REQUIRE(b.GetLine() == std::wstring(L"> "));
    REQUIRE(b.GetUserText() == std::wstring(L""));
    b.DeleteBack();
    REQUIRE(b.GetLine() == std::wstring(L"> "));
    REQUIRE(b.GetUserText() == std::wstring(L""));
}

TEST_CASE("LineBuffer clear")
{
    procdraw::LineBuffer b(L"> ");
    b.Insert('a');
    REQUIRE(b.GetLine() == std::wstring(L"> a"));
    REQUIRE(b.GetUserText() == std::wstring(L"a"));
    b.Clear(L"new> ");
    REQUIRE(b.GetLine() == std::wstring(L"new> "));
    REQUIRE(b.GetUserText() == std::wstring(L""));
    b.Insert('b');
    REQUIRE(b.GetLine() == std::wstring(L"new> b"));
    REQUIRE(b.GetUserText() == std::wstring(L"b"));
    b.DeleteBack();
    REQUIRE(b.GetLine() == std::wstring(L"new> "));
    REQUIRE(b.GetUserText() == std::wstring(L""));
    b.DeleteBack();
    REQUIRE(b.GetLine() == std::wstring(L"new> "));
    REQUIRE(b.GetUserText() == std::wstring(L""));
}
