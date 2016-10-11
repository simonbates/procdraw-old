#include "procdraw_core/interpreter/lisp_memory.h"
#include "procdraw_core/interpreter/lisp_interpreter.h"
#include <functional>
#include <limits>
#include <memory>
#include <string>
#include <unordered_map>

namespace procdraw {

size_t LispObjectHash(const LispObjectPtr& obj);

class LispObject
{
public:
  explicit LispObject(LispObjectType t)
    : Type(t)
  {
  }
  virtual ~LispObject() {}
  const LispObjectType Type;
};

class LispNumber : public LispObject
{
public:
  explicit LispNumber(double val)
    : LispObject(LispObjectType::Number)
    , val_(val)
  {
  }
  double val_;
};

class LispSymbol : public LispObject
{
public:
  LispSymbol(const std::string& name, LispObjectPtr value)
    : LispObject(LispObjectType::Symbol)
    , name_(name)
    , value_(value)
  {
  }
  std::string name_;
  LispObjectPtr value_;
};

class LispCons : public LispObject
{
public:
  LispCons(LispObjectPtr car, LispObjectPtr cdr)
    : LispObject(LispObjectType::Cons)
    , car_(car)
    , cdr_(cdr)
  {
  }
  LispObjectPtr car_;
  LispObjectPtr cdr_;
};

class LispCFunction : public LispObject
{
public:
  LispCFunction(lisp_CFunction cfun, void* data)
    : LispObject(LispObjectType::CFunction)
    , cfun_(cfun)
    , data_(data)
  {
  }
  lisp_CFunction cfun_;
  void* data_;
};

class LispBoolean : public LispObject
{
public:
  explicit LispBoolean(bool val)
    : LispObject(LispObjectType::Boolean)
    , val_(val)
  {
  }
  bool val_;
};

class LispString : public LispObject
{
public:
  explicit LispString(const std::string& str)
    : LispObject(LispObjectType::String)
    , str_(str)
  {
    strHashValue_ = std::hash<std::string>()(str_);
  }
  const std::string str_;
  size_t strHashValue_;
};

class LispDictionary : public LispObject
{
public:
  LispDictionary()
    : LispObject(LispObjectType::Dictionary)
    , dictData_(10, LispObjectHash, LispObjectEq)
  {
  }
  std::unordered_map<LispObjectPtr, LispObjectPtr, decltype(LispObjectHash)*,
                     decltype(LispObjectEq)*>
    dictData_;
};

bool
LispObjectEq(const LispObjectPtr& x, const LispObjectPtr& y)
{
  if (x->Type == LispObjectType::Number && y->Type == LispObjectType::Number) {
    return static_cast<LispNumber*>(x.get())->val_ ==
           static_cast<LispNumber*>(y.get())->val_;
  } else if (x->Type == LispObjectType::String &&
             y->Type == LispObjectType::String) {
    LispString* stringX = static_cast<LispString*>(x.get());
    LispString* stringY = static_cast<LispString*>(y.get());
    if (stringX->strHashValue_ != stringY->strHashValue_) {
      return false;
    } else {
      return stringX->str_ == stringY->str_;
    }
  } else {
    return x.get() == y.get();
  }
}

size_t
LispObjectHash(const LispObjectPtr& obj)
{
  if (obj->Type == LispObjectType::Number) {
    return std::hash<double>()(static_cast<LispNumber*>(obj.get())->val_);
  } else if (obj->Type == LispObjectType::String) {
    return static_cast<LispString*>(obj.get())->strHashValue_;
  } else {
    return std::hash<LispObjectPtr>()(obj);
  }
}

void
LispInterpreter::InitSingletons()
{
  Nil = std::make_shared<LispObject>(LispObjectType::Null);
  True = std::make_shared<LispBoolean>(true);
  False = std::make_shared<LispBoolean>(false);
  Eof = std::make_shared<LispObject>(LispObjectType::Eof);
}

LispObjectPtr
LispInterpreter::MakeNumber(double val)
{
  return std::make_shared<LispNumber>(val);
}

LispObjectPtr
LispInterpreter::MakeSymbol(const std::string& name, LispObjectPtr value)
{
  return std::make_shared<LispSymbol>(name, value);
}

LispObjectPtr
LispInterpreter::Cons(LispObjectPtr car, LispObjectPtr cdr)
{
  return std::make_shared<LispCons>(car, cdr);
}

LispObjectPtr
LispInterpreter::MakeCFunction(lisp_CFunction cfun, void* data)
{
  return std::make_shared<LispCFunction>(cfun, data);
}

LispObjectPtr
LispInterpreter::MakeString(const std::string& str)
{
  return std::make_shared<LispString>(str);
}

LispObjectPtr
LispInterpreter::MakeDict()
{
  return std::make_shared<LispDictionary>();
}

LispObjectType
LispInterpreter::TypeOf(LispObjectPtr obj)
{
  return obj->Type;
}

bool
LispInterpreter::Null(LispObjectPtr obj)
{
  return obj->Type == LispObjectType::Null;
}

bool
LispInterpreter::IsEof(LispObjectPtr obj)
{
  return obj->Type == LispObjectType::Eof;
}

double
LispInterpreter::NumVal(LispObjectPtr obj)
{
  if (obj->Type == LispObjectType::Number) {
    return static_cast<LispNumber*>(obj.get())->val_;
  } else {
    return std::numeric_limits<double>::quiet_NaN();
  }
}

std::string
LispInterpreter::SymbolName(LispObjectPtr symbol)
{
  if (symbol->Type == LispObjectType::Symbol) {
    return static_cast<LispSymbol*>(symbol.get())->name_;
  } else {
    // TODO: Or throw bad type exception?
    return "";
  }
}

LispObjectPtr
LispInterpreter::SymbolValue(LispObjectPtr symbol)
{
  if (symbol->Type == LispObjectType::Symbol) {
    return static_cast<LispSymbol*>(symbol.get())->value_;
  } else {
    // TODO: Or throw bad type exception?
    return Nil;
  }
}

LispObjectPtr
LispInterpreter::SetSymbolValue(LispObjectPtr symbol, LispObjectPtr value)
{
  if (symbol->Type == LispObjectType::Symbol) {
    static_cast<LispSymbol*>(symbol.get())->value_ = value;
    return value;
  } else {
    // TODO: Or throw bad type exception?
    return Nil;
  }
}

LispObjectPtr
LispInterpreter::Car(LispObjectPtr obj)
{
  if (obj->Type == LispObjectType::Cons) {
    return static_cast<LispCons*>(obj.get())->car_;
  } else {
    // TODO: Or throw bad type exception?
    return Nil;
  }
}

LispObjectPtr
LispInterpreter::Cdr(LispObjectPtr obj)
{
  if (obj->Type == LispObjectType::Cons) {
    return static_cast<LispCons*>(obj.get())->cdr_;
  } else {
    // TODO: Or throw bad type exception?
    return Nil;
  }
}

bool
LispInterpreter::BoolVal(LispObjectPtr obj)
{
  switch (obj->Type) {
    case LispObjectType::Boolean:
      return static_cast<LispBoolean*>(obj.get())->val_;
    case LispObjectType::Null:
      return false;
    default:
      return true;
  }
}

std::string
LispInterpreter::StringVal(LispObjectPtr obj)
{
  if (obj->Type == LispObjectType::String) {
    return static_cast<LispString*>(obj.get())->str_;
  } else {
    // TODO: Or throw bad type exception?
    return "";
  }
}

LispObjectPtr
LispInterpreter::Rplaca(LispObjectPtr cons, LispObjectPtr obj)
{
  if (cons->Type == LispObjectType::Cons) {
    static_cast<LispCons*>(cons.get())->car_ = obj;
  }
  // TODO: If not LispObjectType::Cons?
  return cons;
}

LispObjectPtr
LispInterpreter::Rplacd(LispObjectPtr cons, LispObjectPtr obj)
{
  if (cons->Type == LispObjectType::Cons) {
    static_cast<LispCons*>(cons.get())->cdr_ = obj;
  }
  // TODO: If not LispObjectType::Cons?
  return cons;
}

LispObjectPtr
LispInterpreter::Get(LispObjectPtr key, LispObjectPtr dict,
                     LispObjectPtr notFound)
{
  if (dict->Type == LispObjectType::Dictionary) {
    auto dictData = static_cast<LispDictionary*>(dict.get())->dictData_;
    auto found = dictData.find(key);
    if (found != dictData.end()) {
      return found->second;
    } else {
      return notFound;
    }
  }
  // TODO: If not LispObjectType::Dictionary?
  return Nil;
}

LispObjectPtr
LispInterpreter::Get(LispObjectPtr key, LispObjectPtr dict)
{
  return Get(key, dict, Nil);
}

LispObjectPtr
LispInterpreter::Put(LispObjectPtr key, LispObjectPtr val, LispObjectPtr dict)
{
  if (dict->Type == LispObjectType::Dictionary) {
    static_cast<LispDictionary*>(dict.get())->dictData_[key] = val;
    return val;
  }
  // TODO: If not LispObjectType::Dictionary?
  return Nil;
}

LispObjectPtr
LispInterpreter::Keys(LispObjectPtr dict)
{
  LispObjectPtr keys = Nil;
  if (dict->Type == LispObjectType::Dictionary) {
    auto dictData = static_cast<LispDictionary*>(dict.get())->dictData_;
    for (auto it = dictData.begin(); it != dictData.end(); ++it) {
      keys = Cons(it->first, keys);
    }
  }
  return keys;
}

LispObjectPtr
LispInterpreter::Clear(LispObjectPtr dict)
{
  if (dict->Type == LispObjectType::Dictionary) {
    static_cast<LispDictionary*>(dict.get())->dictData_.clear();
    return dict;
  }
  // TODO: Else if not a Dictionary, complain?
  return Nil;
}

LispObjectPtr
LispInterpreter::ApplyCFunction(LispObjectPtr cfun, LispObjectPtr args,
                                LispObjectPtr env)
{
  if (cfun->Type == LispObjectType::CFunction) {
    LispCFunction* f = static_cast<LispCFunction*>(cfun.get());
    return f->cfun_(this, args, env, f->data_);
  } else {
    // TODO: Or throw bad type exception?
    return Nil;
  }
}
}
