#include "procdraw_core/interpreter/lisp_interpreter.h"
#include "procdraw_core/interpreter/lisp_functions.h"
#include <cmath>
#include <string>
#include <vector>

namespace procdraw {

LispInterpreter::LispInterpreter()
{
  InitSingletons();
  symbols_ = Nil;
  // Special forms
  S_DEF = SymbolRef("def");
  S_IF = SymbolRef("if");
  S_LAMBDA = SymbolRef("lambda");
  S_PROGN = SymbolRef("progn");
  S_QUOTE = SymbolRef("quote");
  S_SETQ = SymbolRef("setq");
  // Functions
  SetGlobalCFunction("*", lisp_Product, nullptr);
  SetGlobalCFunction("+", lisp_Sum, nullptr);
  SetGlobalCFunction("-", lisp_Difference, nullptr);
  SetGlobalCFunction("/", lisp_Quotient, nullptr);
  SetGlobalCFunction("apply", lisp_Apply, nullptr);
  SetGlobalCFunction("assoc", lisp_Assoc, nullptr);
  SetGlobalCFunction("car", lisp_Car, nullptr);
  SetGlobalCFunction("cdr", lisp_Cdr, nullptr);
  SetGlobalCFunction("clear", lisp_Clear, nullptr);
  SetGlobalCFunction("cons", lisp_Cons, nullptr);
  SetGlobalCFunction("eq", lisp_Eq, nullptr);
  SetGlobalCFunction("functionp", lisp_Functionp, nullptr);
  SetGlobalCFunction("get", lisp_Get, nullptr);
  SetGlobalCFunction("keys", lisp_Keys, nullptr);
  SetGlobalCFunction("lerp", lisp_Lerp, nullptr);
  SetGlobalCFunction("make-dict", lisp_MakeDict, nullptr);
  SetGlobalCFunction("map-range", lisp_MapRange, nullptr);
  SetGlobalCFunction("memb", lisp_Memb, nullptr);
  SetGlobalCFunction("norm", lisp_Norm, nullptr);
  SetGlobalCFunction("not", lisp_Not, nullptr);
  SetGlobalCFunction("put", lisp_Put, nullptr);
  SetGlobalCFunction("putassoc", lisp_Putassoc, nullptr);
  SetGlobalCFunction("wrap", lisp_Wrap, nullptr);
  // Constants
  Set(SymbolRef("pi"), MakeNumber(M_PI), Nil);
}

LispObjectPtr
LispInterpreter::SymbolRef(const std::string& name)
{
  LispObjectPtr n = symbols_;
  while (!Null(n)) {
    auto symbol = Car(n);
    if (SymbolName(symbol) == name) {
      return symbol;
    }
    n = Cdr(n);
  }
  auto symbol = MakeSymbol(name, Nil);
  symbols_ = Cons(symbol, symbols_);
  return symbol;
}

LispObjectPtr
LispInterpreter::SetGlobalCFunction(const std::string& name,
                                    lisp_CFunction cfun, void* data)
{
  return Set(SymbolRef(name), MakeCFunction(cfun, data), Nil);
}

LispObjectPtr
LispInterpreter::Apply(LispObjectPtr fun, LispObjectPtr args, LispObjectPtr env)
{
  switch (TypeOf(fun)) {
    case LispObjectType::CFunction:
      return ApplyCFunction(fun, args, env);
    case LispObjectType::Cons:
      return Progn(Cddr(fun), Bind(Cadr(fun), args, env));
    default:
      // TODO complain
      return Nil;
  }
}

LispObjectPtr
LispInterpreter::Assoc1(LispObjectPtr key, LispObjectPtr alist, bool* found)
{
  LispObjectPtr next = alist;
  LispObjectPtr prev = alist;
  while (!Null(next)) {
    auto association = Car(next);
    if (LispObjectEq(key, Car(association))) {
      *found = true;
      return association;
    }
    prev = next;
    next = Cdr(next);
  }
  *found = false;
  return prev;
}

LispObjectPtr
LispInterpreter::Assoc(LispObjectPtr key, LispObjectPtr alist)
{
  bool found;
  auto pair = Assoc1(key, alist, &found);
  if (found) {
    return pair;
  } else {
    return Nil;
  }
}

bool
LispInterpreter::Atom(LispObjectPtr obj)
{
  return TypeOf(obj) != LispObjectType::Cons;
}

LispObjectPtr
LispInterpreter::Bind(LispObjectPtr vars, LispObjectPtr args, LispObjectPtr env)
{
  // TODO: verify that vars and args are the same length
  auto v = vars;
  auto a = args;
  while (!Null(v)) {
    env = Cons(Cons(Car(v), Car(a)), env);
    v = Cdr(v);
    a = Cdr(a);
  }
  return env;
}

LispObjectPtr
LispInterpreter::BoolToLisp(bool b)
{
  return b ? True : False;
}

LispObjectPtr
LispInterpreter::Caar(LispObjectPtr obj)
{
  return Car(Car(obj));
}

LispObjectPtr
LispInterpreter::Cadr(LispObjectPtr obj)
{
  return Car(Cdr(obj));
}

LispObjectPtr
LispInterpreter::Cdar(LispObjectPtr obj)
{
  return Cdr(Car(obj));
}

LispObjectPtr
LispInterpreter::Cddr(LispObjectPtr obj)
{
  return Cdr(Cdr(obj));
}

LispObjectPtr
LispInterpreter::Caddr(LispObjectPtr obj)
{
  return Car(Cdr(Cdr(obj)));
}

LispObjectPtr
LispInterpreter::Cadddr(LispObjectPtr obj)
{
  return Car(Cdr(Cdr(Cdr(obj))));
}

LispObjectPtr
LispInterpreter::Caddddr(LispObjectPtr obj)
{
  return Car(Cdr(Cdr(Cdr(Cdr(obj)))));
}

LispObjectPtr
LispInterpreter::Call(const std::string& name)
{
  return Apply(Value(SymbolRef(name), Nil), Nil, Nil);
}

BalancedState
LispInterpreter::CheckBalanced(const std::string& str)
{
  return reader_.CheckBalanced(str);
}

LispObjectPtr
LispInterpreter::Eval(LispObjectPtr exp, LispObjectPtr env)
{
  if (Atom(exp)) {
    switch (TypeOf(exp)) {
      case LispObjectType::Boolean:
      case LispObjectType::Null:
      case LispObjectType::Number:
      case LispObjectType::String:
      case LispObjectType::Eof:
        return exp;
      default:
        return Value(exp, env);
    }
  } else {
    auto first = Car(exp);
    if (LispObjectEq(first, S_DEF)) {
      return Set(Cadr(exp), Cons(S_LAMBDA, Cddr(exp)), env);
    } else if (LispObjectEq(first, S_IF)) {
      return Evif(Cdr(exp), env);
    } else if (LispObjectEq(first, S_LAMBDA)) {
      return exp;
    } else if (LispObjectEq(first, S_PROGN)) {
      return Progn(Cdr(exp), env);
    } else if (LispObjectEq(first, S_QUOTE)) {
      return Cadr(exp);
    } else if (LispObjectEq(first, S_SETQ)) {
      return Set(Cadr(exp), Eval(Caddr(exp), env), env);
    } else {
      return Apply(Eval(first, env), Evlis(Cdr(exp), env), env);
    }
  }

  return Nil;
}

LispObjectPtr
LispInterpreter::Eval(LispObjectPtr exp)
{
  return Eval(exp, Nil);
}

LispObjectPtr
LispInterpreter::Evif(LispObjectPtr arglist, LispObjectPtr env)
{
  if (BoolVal(Eval(Car(arglist), env))) {
    return Eval(Cadr(arglist), env);
  } else {
    return Eval(Caddr(arglist), env);
  }
}

LispObjectPtr
LispInterpreter::Evlis(LispObjectPtr arglist, LispObjectPtr env)
{
  if (Null(arglist)) {
    return Nil;
  } else {
    return Cons(Eval(Car(arglist), env), Evlis(Cdr(arglist), env));
  }
}

bool
LispInterpreter::Functionp(LispObjectPtr obj)
{
  auto type = TypeOf(obj);
  return (type == LispObjectType::Cons && LispObjectEq(Car(obj), S_LAMBDA)) ||
         type == LispObjectType::CFunction;
}

LispObjectPtr
LispInterpreter::MakeList(std::vector<LispObjectPtr> objs)
{
  auto list1 = Nil;
  for (auto it = objs.rbegin(); it != objs.rend(); ++it) {
    list1 = Cons(*it, list1);
  }
  return list1;
}

bool
LispInterpreter::Memb(LispObjectPtr obj, LispObjectPtr list)
{
  for (LispObjectPtr n = list; !Null(n); n = Cdr(n)) {
    if (LispObjectEq(obj, Car(n))) {
      return true;
    }
  }
  return false;
}

LispObjectPtr
LispInterpreter::Not(LispObjectPtr obj)
{
  if (BoolVal(obj)) {
    return False;
  } else {
    return True;
  }
}

std::string
LispInterpreter::PrettyPrintToString(LispObjectPtr obj, int margin)
{
  return prettyPrinter_.PrintToString(this, obj, margin);
}

std::string
LispInterpreter::PrintToString(LispObjectPtr obj)
{
  // TODO: Using a value of 1000 here is a kludge -- implement proper support
  // for printing without newlines
  return prettyPrinter_.PrintToString(this, obj, 1000);
}

LispObjectPtr
LispInterpreter::Progn(LispObjectPtr actions, LispObjectPtr env)
{
  LispObjectPtr result = Nil;
  for (LispObjectPtr n = actions; !Null(n); n = Cdr(n)) {
    result = Eval(Car(n), env);
  }
  return result;
}

LispObjectPtr
LispInterpreter::Putassoc(LispObjectPtr key, LispObjectPtr val,
                          LispObjectPtr alist)
{
  if (TypeOf(alist) == LispObjectType::Cons) {
    bool found;
    auto pair = Assoc1(key, alist, &found);
    if (found) {
      Rplacd(pair, val);
    } else {
      // The object returned from Assoc1 is the last element in the list
      // Add the new (key, val) association onto the end
      Rplacd(pair, Cons(Cons(key, val), Nil));
    }
  } else {
    // TODO complain
  }
  return val;
}

LispObjectPtr
LispInterpreter::Read(const std::string& str)
{
  return reader_.Read(this, str);
}

LispObjectPtr
LispInterpreter::Set(LispObjectPtr symbol, LispObjectPtr value,
                     LispObjectPtr env)
{
  auto binding = Assoc(symbol, env);
  if (Null(binding)) {
    SetSymbolValue(symbol, value);
  } else {
    Rplacd(binding, value);
  }
  return value;
}

LispObjectPtr
LispInterpreter::Value(LispObjectPtr symbol, LispObjectPtr env)
{
  auto binding = Assoc(symbol, env);
  if (Null(binding)) {
    return SymbolValue(symbol);
  } else {
    return Cdr(binding);
  }
}
}
