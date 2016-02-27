#include "line_buffer.h"
#include "catch.hpp"

TEST_CASE("LineBuffer void constructor")
{
    procdraw::LineBuffer b;
    REQUIRE(b.GetLine() == std::string(""));
    REQUIRE(b.GetUserText() == std::string(""));
    b.Insert('a');
    REQUIRE(b.GetLine() == std::string("a"));
    REQUIRE(b.GetUserText() == std::string("a"));
}

TEST_CASE("LineBuffer empty prompt")
{
    procdraw::LineBuffer b("");
    REQUIRE(b.GetLine() == std::string(""));
    REQUIRE(b.GetUserText() == std::string(""));
}

TEST_CASE("LineBuffer non empty prompt")
{
    procdraw::LineBuffer b("> ");
    REQUIRE(b.GetLine() == std::string("> "));
    REQUIRE(b.GetUserText() == std::string(""));
}

TEST_CASE("LineBuffer insert and delete")
{
    procdraw::LineBuffer b("> ");
    REQUIRE(b.GetLine() == std::string("> "));
    REQUIRE(b.GetUserText() == std::string(""));
    b.Insert('a');
    REQUIRE(b.GetLine() == std::string("> a"));
    REQUIRE(b.GetUserText() == std::string("a"));
    b.Insert('b');
    REQUIRE(b.GetLine() == std::string("> ab"));
    REQUIRE(b.GetUserText() == std::string("ab"));
    b.DeleteBack();
    REQUIRE(b.GetLine() == std::string("> a"));
    REQUIRE(b.GetUserText() == std::string("a"));
    b.DeleteBack();
    REQUIRE(b.GetLine() == std::string("> "));
    REQUIRE(b.GetUserText() == std::string(""));
}

TEST_CASE("LineBuffer cannot delete prompt")
{
    procdraw::LineBuffer b("> ");
    REQUIRE(b.GetLine() == std::string("> "));
    REQUIRE(b.GetUserText() == std::string(""));
    b.Insert('a');
    REQUIRE(b.GetLine() == std::string("> a"));
    REQUIRE(b.GetUserText() == std::string("a"));
    b.DeleteBack();
    REQUIRE(b.GetLine() == std::string("> "));
    REQUIRE(b.GetUserText() == std::string(""));
    b.DeleteBack();
    REQUIRE(b.GetLine() == std::string("> "));
    REQUIRE(b.GetUserText() == std::string(""));
}

TEST_CASE("LineBuffer clear")
{
    procdraw::LineBuffer b("> ");
    b.Insert('a');
    REQUIRE(b.GetLine() == std::string("> a"));
    REQUIRE(b.GetUserText() == std::string("a"));
    b.Clear("new> ");
    REQUIRE(b.GetLine() == std::string("new> "));
    REQUIRE(b.GetUserText() == std::string(""));
    b.Insert('b');
    REQUIRE(b.GetLine() == std::string("new> b"));
    REQUIRE(b.GetUserText() == std::string("b"));
    b.DeleteBack();
    REQUIRE(b.GetLine() == std::string("new> "));
    REQUIRE(b.GetUserText() == std::string(""));
    b.DeleteBack();
    REQUIRE(b.GetLine() == std::string("new> "));
    REQUIRE(b.GetUserText() == std::string(""));
}

TEST_CASE("LineBuffer move cursor with prompt")
{
    procdraw::LineBuffer b("> ");
    REQUIRE(b.GetCursorPos() == 2);
    b.BackwardChar();
    REQUIRE(b.GetCursorPos() == 2);
    b.ForwardChar();
    REQUIRE(b.GetCursorPos() == 2);
    b.Insert('a');
    b.Insert('b');
    b.Insert('c');
    REQUIRE(b.GetCursorPos() == 5);
    b.BackwardChar();
    REQUIRE(b.GetCursorPos() == 4);
    b.BackwardChar();
    REQUIRE(b.GetCursorPos() == 3);
    b.BackwardChar();
    REQUIRE(b.GetCursorPos() == 2);
    b.BackwardChar();
    REQUIRE(b.GetCursorPos() == 2);
    b.ForwardChar();
    REQUIRE(b.GetCursorPos() == 3);
    b.ForwardChar();
    REQUIRE(b.GetCursorPos() == 4);
    b.ForwardChar();
    REQUIRE(b.GetCursorPos() == 5);
    b.ForwardChar();
    REQUIRE(b.GetCursorPos() == 5);
}

TEST_CASE("LineBuffer move cursor without prompt")
{
    procdraw::LineBuffer b;
    REQUIRE(b.GetCursorPos() == 0);
    b.BackwardChar();
    REQUIRE(b.GetCursorPos() == 0);
    b.ForwardChar();
    REQUIRE(b.GetCursorPos() == 0);
    b.Insert('a');
    b.Insert('b');
    b.Insert('c');
    REQUIRE(b.GetCursorPos() == 3);
    b.BackwardChar();
    b.BackwardChar();
    b.BackwardChar();
    b.BackwardChar();
    REQUIRE(b.GetCursorPos() == 0);
    b.ForwardChar();
    b.ForwardChar();
    b.ForwardChar();
    b.ForwardChar();
    REQUIRE(b.GetCursorPos() == 3);
}

TEST_CASE("LineBuffer cursor edit")
{
    procdraw::LineBuffer b("> ");
    REQUIRE(b.GetCursorPos() == 2);
    b.Insert('a');
    b.DeleteBack();
    REQUIRE(b.GetCursorPos() == 2);
    b.Insert('b');
    b.BackwardChar();
    REQUIRE(b.GetCursorPos() == 2);
    b.Insert('a');
    REQUIRE(b.GetUserText() == std::string("ab"));
    REQUIRE(b.GetCursorPos() == 3);
    b.Insert('c');
    REQUIRE(b.GetUserText() == std::string("acb"));
    REQUIRE(b.GetCursorPos() == 4);
    b.DeleteBack();
    REQUIRE(b.GetUserText() == std::string("ab"));
    REQUIRE(b.GetCursorPos() == 3);
}
