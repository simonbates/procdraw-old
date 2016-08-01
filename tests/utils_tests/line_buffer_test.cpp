#include "procdraw/utils/line_buffer.h"
#include "gtest/gtest.h"

TEST(LineBufferTest, VoidConstructor)
{
    procdraw::LineBuffer b;
    EXPECT_EQ(std::string(""), b.GetLine());
    EXPECT_EQ(std::string(""), b.GetUserText());
    b.Insert('a');
    EXPECT_EQ(std::string("a"), b.GetLine());
    EXPECT_EQ(std::string("a"), b.GetUserText());
}

TEST(LineBufferTest, EmptyPrompt)
{
    procdraw::LineBuffer b("");
    EXPECT_EQ(std::string(""), b.GetLine());
    EXPECT_EQ(std::string(""), b.GetUserText());
}

TEST(LineBufferTest, NonEmptyPrompt)
{
    procdraw::LineBuffer b("> ");
    EXPECT_EQ(std::string("> "), b.GetLine());
    EXPECT_EQ(std::string(""), b.GetUserText());
}

TEST(LineBufferTest, InsertAndDelete)
{
    procdraw::LineBuffer b("> ");
    EXPECT_EQ(std::string("> "), b.GetLine());
    EXPECT_EQ(std::string(""), b.GetUserText());
    b.Insert('a');
    EXPECT_EQ(std::string("> a"), b.GetLine());
    EXPECT_EQ(std::string("a"), b.GetUserText());
    b.Insert('b');
    EXPECT_EQ(std::string("> ab"), b.GetLine());
    EXPECT_EQ(std::string("ab"), b.GetUserText());
    b.DeleteBack();
    EXPECT_EQ(std::string("> a"), b.GetLine());
    EXPECT_EQ(std::string("a"), b.GetUserText());
    b.DeleteBack();
    EXPECT_EQ(std::string("> "), b.GetLine());
    EXPECT_EQ(std::string(""), b.GetUserText());
}

TEST(LineBufferTest, CannotDeletePrompt)
{
    procdraw::LineBuffer b("> ");
    EXPECT_EQ(std::string("> "), b.GetLine());
    EXPECT_EQ(std::string(""), b.GetUserText());
    b.Insert('a');
    EXPECT_EQ(std::string("> a"), b.GetLine());
    EXPECT_EQ(std::string("a"), b.GetUserText());
    b.DeleteBack();
    EXPECT_EQ(std::string("> "), b.GetLine());
    EXPECT_EQ(std::string(""), b.GetUserText());
    b.DeleteBack();
    EXPECT_EQ(std::string("> "), b.GetLine());
    EXPECT_EQ(std::string(""), b.GetUserText());
}

TEST(LineBufferTest, Clear)
{
    procdraw::LineBuffer b("> ");
    b.Insert('a');
    EXPECT_EQ(std::string("> a"), b.GetLine());
    EXPECT_EQ(std::string("a"), b.GetUserText());
    b.Clear("new> ");
    EXPECT_EQ(std::string("new> "), b.GetLine());
    EXPECT_EQ(std::string(""), b.GetUserText());
    b.Insert('b');
    EXPECT_EQ(std::string("new> b"), b.GetLine());
    EXPECT_EQ(std::string("b"), b.GetUserText());
    b.DeleteBack();
    EXPECT_EQ(std::string("new> "), b.GetLine());
    EXPECT_EQ(std::string(""), b.GetUserText());
    b.DeleteBack();
    EXPECT_EQ(std::string("new> "), b.GetLine());
    EXPECT_EQ(std::string(""), b.GetUserText());
}

TEST(LineBufferTest, MoveCursorWithPrompt)
{
    procdraw::LineBuffer b("> ");
    EXPECT_EQ(2, b.GetCursorPos());
    b.BackwardChar();
    EXPECT_EQ(2, b.GetCursorPos());
    b.ForwardChar();
    EXPECT_EQ(2, b.GetCursorPos());
    b.Insert('a');
    b.Insert('b');
    b.Insert('c');
    EXPECT_EQ(5, b.GetCursorPos());
    b.BackwardChar();
    EXPECT_EQ(4, b.GetCursorPos());
    b.BackwardChar();
    EXPECT_EQ(3, b.GetCursorPos());
    b.BackwardChar();
    EXPECT_EQ(2, b.GetCursorPos());
    b.BackwardChar();
    EXPECT_EQ(2, b.GetCursorPos());
    b.ForwardChar();
    EXPECT_EQ(3, b.GetCursorPos());
    b.ForwardChar();
    EXPECT_EQ(4, b.GetCursorPos());
    b.ForwardChar();
    EXPECT_EQ(5, b.GetCursorPos());
    b.ForwardChar();
    EXPECT_EQ(5, b.GetCursorPos());
}

TEST(LineBufferTest, MoveCursorWithoutPrompt)
{
    procdraw::LineBuffer b;
    EXPECT_EQ(0, b.GetCursorPos());
    b.BackwardChar();
    EXPECT_EQ(0, b.GetCursorPos());
    b.ForwardChar();
    EXPECT_EQ(0, b.GetCursorPos());
    b.Insert('a');
    b.Insert('b');
    b.Insert('c');
    EXPECT_EQ(3, b.GetCursorPos());
    b.BackwardChar();
    b.BackwardChar();
    b.BackwardChar();
    b.BackwardChar();
    EXPECT_EQ(0, b.GetCursorPos());
    b.ForwardChar();
    b.ForwardChar();
    b.ForwardChar();
    b.ForwardChar();
    EXPECT_EQ(3, b.GetCursorPos());
}

TEST(LineBufferTest, CursorEdit)
{
    procdraw::LineBuffer b("> ");
    EXPECT_EQ(2, b.GetCursorPos());
    b.Insert('a');
    b.DeleteBack();
    EXPECT_EQ(2, b.GetCursorPos());
    b.Insert('b');
    b.BackwardChar();
    EXPECT_EQ(2, b.GetCursorPos());
    b.Insert('a');
    EXPECT_EQ(std::string("ab"), b.GetUserText());
    EXPECT_EQ(3, b.GetCursorPos());
    b.Insert('c');
    EXPECT_EQ(std::string("acb"), b.GetUserText());
    EXPECT_EQ(4, b.GetCursorPos());
    b.DeleteBack();
    EXPECT_EQ(std::string("ab"), b.GetUserText());
    EXPECT_EQ(3, b.GetCursorPos());
}
