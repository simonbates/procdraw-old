#include "catch.hpp"
#include "procdraw/interpreter/lisp_interpreter.h"
#include "procdraw/interpreter/lisp_reader.h"

TEST_CASE("Lisp reader")
{

  procdraw::LispReader reader;
  procdraw::LispInterpreter L;

  SECTION("An integer without a sign is read as a number")
  {
    auto obj = reader.Read(&L, "42");
    REQUIRE(L.TypeOf(obj) == procdraw::LispObjectType::Number);
    REQUIRE(L.NumVal(obj) == 42);
  }

  SECTION("An integer with a positive sign is read as a number")
  {
    auto obj = reader.Read(&L, "+42");
    REQUIRE(L.TypeOf(obj) == procdraw::LispObjectType::Number);
    REQUIRE(L.NumVal(obj) == 42);
  }

  SECTION("An integer with a negative sign is read as a number")
  {
    auto obj = reader.Read(&L, "-42");
    REQUIRE(L.TypeOf(obj) == procdraw::LispObjectType::Number);
    REQUIRE(L.NumVal(obj) == -42);
  }

  SECTION("An identifier starting with a letter is read as a symbol")
  {
    auto obj = reader.Read(&L, "HELLO-WORLD-1");
    REQUIRE(L.TypeOf(obj) == procdraw::LispObjectType::Symbol);
    REQUIRE(L.SymbolName(obj) == "HELLO-WORLD-1");
  }

  SECTION("An expression starting with $ is read as a sigval")
  {
    auto obj = reader.Read(&L, "$foo");
    REQUIRE(L.TypeOf(obj) == procdraw::LispObjectType::Cons);
    REQUIRE(L.SymbolName(L.Car(obj)) == "sigval");
    REQUIRE(L.SymbolName(L.Cadr(obj)) == "foo");
    REQUIRE(L.Null(L.Cddr(obj)));
  }

  SECTION("An expression starting with ' is read as a quote")
  {
    auto obj = reader.Read(&L, "'42");
    REQUIRE(L.TypeOf(obj) == procdraw::LispObjectType::Cons);
    REQUIRE(L.SymbolName(L.Car(obj)) == "quote");
    REQUIRE(L.NumVal(L.Cadr(obj)) == 42);
    REQUIRE(L.Null(L.Cddr(obj)));
  }

  SECTION("A star char is read as a symbol")
  {
    auto obj = reader.Read(&L, "*");
    REQUIRE(L.TypeOf(obj) == procdraw::LispObjectType::Symbol);
    REQUIRE(L.SymbolName(obj) == "*");
  }

  SECTION("A plus char is read as a symbol")
  {
    auto obj = reader.Read(&L, "(+ 42)");
    REQUIRE(L.TypeOf(L.Car(obj)) == procdraw::LispObjectType::Symbol);
    REQUIRE(L.SymbolName(L.Car(obj)) == "+");
    REQUIRE(L.TypeOf(L.Cadr(obj)) == procdraw::LispObjectType::Number);
    REQUIRE(L.NumVal(L.Cadr(obj)) == 42);
  }

  SECTION("A hyphen-minus char is read as a symbol")
  {
    auto obj = reader.Read(&L, "(- 42)");
    REQUIRE(L.TypeOf(L.Car(obj)) == procdraw::LispObjectType::Symbol);
    REQUIRE(L.SymbolName(L.Car(obj)) == "-");
    REQUIRE(L.TypeOf(L.Cadr(obj)) == procdraw::LispObjectType::Number);
    REQUIRE(L.NumVal(L.Cadr(obj)) == 42);
  }

  SECTION("A slash char is read as a symbol")
  {
    auto obj = reader.Read(&L, "/");
    REQUIRE(L.TypeOf(obj) == procdraw::LispObjectType::Symbol);
    REQUIRE(L.SymbolName(obj) == "/");
  }

  SECTION("\"=>\" is read as a symbol")
  {
    auto obj = reader.Read(&L, "(=>42)");
    REQUIRE(L.TypeOf(L.Car(obj)) == procdraw::LispObjectType::Symbol);
    REQUIRE(L.SymbolName(L.Car(obj)) == "=>");
    REQUIRE(L.NumVal(L.Cadr(obj)) == 42);
  }

  SECTION("\"nil\" is read as nil")
  {
    auto obj = reader.Read(&L, "nil");
    REQUIRE(L.Null(obj));
  }

  SECTION("Empty list is read as nil")
  {
    auto obj = reader.Read(&L, "()");
    REQUIRE(L.Null(obj));
  }

  SECTION("A list with a single item can be read")
  {
    auto obj = reader.Read(&L, "(42)");
    REQUIRE(L.TypeOf(obj) == procdraw::LispObjectType::Cons);
    REQUIRE(L.NumVal(L.Car(obj)) == 42);
    REQUIRE(L.Null(L.Cdr(obj)));
  }

  // +---+  +---+  +---+  +---+
  // |1|*|->|2|*|->|3|*|->|4|/|
  // +---+  +---+  +---+  +---+
  //
  SECTION("A list with multiple items can be read")
  {
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
  SECTION("A list with an embedded list can be read")
  {
    auto list1 = reader.Read(&L, "((1 2) 3 4)");
    REQUIRE(L.TypeOf(list1) == procdraw::LispObjectType::Cons);
    REQUIRE(L.NumVal(L.Car(L.Car(list1))) == 1);
    REQUIRE(L.NumVal(L.Car(L.Cdr(L.Car(list1)))) == 2);
    REQUIRE(L.Null(L.Cdr(L.Cdr(L.Car(list1)))));
    REQUIRE(L.NumVal(L.Car(L.Cdr(list1))) == 3);
    REQUIRE(L.NumVal(L.Car(L.Cdr(L.Cdr(list1)))) == 4);
    REQUIRE(L.Null(L.Cdr(L.Cdr(L.Cdr(list1)))));
  }

  SECTION("A dotted pair can be read by itself")
  {
    auto obj = reader.Read(&L, "(2 . 3)");
    REQUIRE(L.NumVal(L.Car(obj)) == 2);
    REQUIRE(L.NumVal(L.Cdr(obj)) == 3);
  }

  // +---+  +---+
  // |*|*|->|4|/|
  // +---+  +---+
  //  |
  //  v
  // +---+
  // |2|3|
  // +---+
  //
  SECTION("A dotted pair can be read within a list")
  {
    auto obj = reader.Read(&L, "((2 . 3) 4)");
    REQUIRE(L.NumVal(L.Car(L.Car(obj))) == 2);
    REQUIRE(L.NumVal(L.Cdr(L.Car(obj))) == 3);
    REQUIRE(L.NumVal(L.Car(L.Cdr(obj))) == 4);
    REQUIRE(L.Null(L.Cdr(L.Cdr(obj))));
  }

  // +---+  +---+
  // |2|*|->|3|4|
  // +---+  +---+
  //
  SECTION("A dotted pair can be read at the end of a list")
  {
    auto obj = reader.Read(&L, "(2 3 . 4)");
    REQUIRE(L.NumVal(L.Car(obj)) == 2);
    REQUIRE(L.NumVal(L.Car(L.Cdr(obj))) == 3);
    REQUIRE(L.NumVal(L.Cdr(L.Cdr(obj))) == 4);
  }

  SECTION("A reader may be used for multiple serial reads")
  {
    auto a = reader.Read(&L, "HELLO");
    REQUIRE(L.SymbolName(a) == "HELLO");
    auto b = reader.Read(&L, "42");
    REQUIRE(L.NumVal(b) == 42);
    auto c = reader.Read(&L, "(LIST 10 20)");
    REQUIRE(L.SymbolName(L.Car(c)) == "LIST");
    REQUIRE(L.NumVal(L.Car(L.Cdr(c))) == 10);
    REQUIRE(L.NumVal(L.Car(L.Cdr(L.Cdr(c)))) == 20);
  }

  SECTION("\"true\" is read as Boolean true")
  {
    auto obj = reader.Read(&L, "true");
    REQUIRE(L.TypeOf(obj) == procdraw::LispObjectType::Boolean);
    REQUIRE(L.BoolVal(obj));
  }

  SECTION("\"false\" is read as Boolean false")
  {
    auto obj = reader.Read(&L, "false");
    REQUIRE(L.TypeOf(obj) == procdraw::LispObjectType::Boolean);
    REQUIRE_FALSE(L.BoolVal(obj));
  }

  SECTION("A non-empty String can be read")
  {
    auto obj = reader.Read(&L, "\"some string\"");
    REQUIRE(L.TypeOf(obj) == procdraw::LispObjectType::String);
    REQUIRE(L.StringVal(obj) == "some string");
  }

  SECTION("An empty String is read is an empty string")
  {
    auto obj = reader.Read(&L, "\"\"");
    REQUIRE(L.TypeOf(obj) == procdraw::LispObjectType::String);
    REQUIRE(L.StringVal(obj) == "");
  }

  SECTION("A list of strings and numbers can be read")
  {
    auto obj = reader.Read(&L, "(\"some string\" 42 \"\" 2)");
    REQUIRE(L.StringVal(L.Car(obj)) == "some string");
    REQUIRE(L.NumVal(L.Cadr(obj)) == 42);
    REQUIRE(L.StringVal(L.Caddr(obj)) == "");
    REQUIRE(L.NumVal(L.Cadddr(obj)) == 2);
  }

  SECTION("Reading a non-closed String throws an exception")
  {
    REQUIRE_THROWS(reader.Read(&L, "\""));
    REQUIRE_THROWS(reader.Read(&L, "\"a"));
  }

  SECTION("Reading an incomplete expression returns eof")
  {
    REQUIRE(L.TypeOf(reader.Read(&L, "")) == procdraw::LispObjectType::Eof);
    REQUIRE(L.TypeOf(reader.Read(&L, "(")) == procdraw::LispObjectType::Eof);
    REQUIRE(L.TypeOf(reader.Read(&L, "(1")) == procdraw::LispObjectType::Eof);
    REQUIRE(L.TypeOf(reader.Read(&L, "(1 2")) == procdraw::LispObjectType::Eof);
    REQUIRE(L.TypeOf(reader.Read(&L, "(1 (2")) ==
            procdraw::LispObjectType::Eof);
  }

  SECTION("Checking a balanced expression returns balanced")
  {
    REQUIRE(reader.CheckBalanced("") == procdraw::BalancedState::Balanced);
    REQUIRE(reader.CheckBalanced("42") == procdraw::BalancedState::Balanced);
    REQUIRE(reader.CheckBalanced("()") == procdraw::BalancedState::Balanced);
    REQUIRE(reader.CheckBalanced("(A B C)") ==
            procdraw::BalancedState::Balanced);
    REQUIRE(reader.CheckBalanced("(A (1 2))") ==
            procdraw::BalancedState::Balanced);
    REQUIRE(reader.CheckBalanced("\"\"") == procdraw::BalancedState::Balanced);
    REQUIRE(reader.CheckBalanced("\"abc\"") ==
            procdraw::BalancedState::Balanced);
  }

  SECTION("Checking a non-closed expression returns not closed")
  {
    REQUIRE(reader.CheckBalanced("(") == procdraw::BalancedState::NotClosed);
    REQUIRE(reader.CheckBalanced("((") == procdraw::BalancedState::NotClosed);
    REQUIRE(reader.CheckBalanced("(42") == procdraw::BalancedState::NotClosed);
    REQUIRE(reader.CheckBalanced("(A B C") ==
            procdraw::BalancedState::NotClosed);
    REQUIRE(reader.CheckBalanced("(A (1 2") ==
            procdraw::BalancedState::NotClosed);
    REQUIRE(reader.CheckBalanced("(A (1 2)") ==
            procdraw::BalancedState::NotClosed);
  }

  SECTION("Checking a non-closed string returns not closed")
  {
    REQUIRE(reader.CheckBalanced("\"") == procdraw::BalancedState::NotClosed);
    REQUIRE(reader.CheckBalanced("\"abc") ==
            procdraw::BalancedState::NotClosed);
    REQUIRE(reader.CheckBalanced("(\"") == procdraw::BalancedState::NotClosed);
    REQUIRE(reader.CheckBalanced("(42 \"") ==
            procdraw::BalancedState::NotClosed);
    REQUIRE(reader.CheckBalanced("(42 (\"") ==
            procdraw::BalancedState::NotClosed);
  }

  SECTION("Checking an expression with too many closing parens returns too "
          "many closing parens")
  {
    REQUIRE(reader.CheckBalanced(")") ==
            procdraw::BalancedState::TooManyClosingParens);
    REQUIRE(reader.CheckBalanced("))") ==
            procdraw::BalancedState::TooManyClosingParens);
    REQUIRE(reader.CheckBalanced("(42))") ==
            procdraw::BalancedState::TooManyClosingParens);
    REQUIRE(reader.CheckBalanced("(A B C))") ==
            procdraw::BalancedState::TooManyClosingParens);
    REQUIRE(reader.CheckBalanced("(A (1 2)))") ==
            procdraw::BalancedState::TooManyClosingParens);
  }
}
