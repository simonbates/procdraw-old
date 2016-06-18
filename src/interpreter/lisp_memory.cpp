#include "lisp_memory.h"
#include "lisp_interpreter.h"
#include <limits>
#include <string>
#include <unordered_map>

namespace procdraw {

    class LispObject {
    public:
        LispObject(LispObjectType t) : Type(t) { }
        virtual ~LispObject() { }
        const LispObjectType Type;
    };

    class LispNumber : public LispObject {
    public:
        LispNumber(double val) :
            LispObject(LispObjectType::Number),
            val_(val) { }
        double val_;
    };

    class LispSymbol : public LispObject {
    public:
        LispSymbol(const std::string &name, LispObjectPtr value) :
            LispObject(LispObjectType::Symbol),
            name_(name), value_(value) { }
        std::string name_;
        LispObjectPtr value_;
    };

    class LispCons : public LispObject {
    public:
        LispCons(LispObjectPtr car, LispObjectPtr cdr) :
            LispObject(LispObjectType::Cons),
            car_(car), cdr_(cdr) { }
        LispObjectPtr car_;
        LispObjectPtr cdr_;
    };

    class LispCFunction : public LispObject {
    public:
        LispCFunction(lisp_CFunction cfun, void *data) :
            LispObject(LispObjectType::CFunction),
            cfun_(cfun),
            data_(data) { }
        lisp_CFunction cfun_;
        void *data_;
    };

    class LispBoolean : public LispObject {
    public:
        LispBoolean(bool val) :
            LispObject(LispObjectType::Boolean),
            val_(val) { }
        bool val_;
    };

    class LispString : public LispObject {
    public:
        LispString(const std::string &str) :
            LispObject(LispObjectType::String),
            str_(str)
        {
            strHashValue_ = std::hash<std::string>()(str_);
        }
        const std::string str_;
        size_t strHashValue_;
    };

    void LispInterpreter::InitSingletons()
    {
        Nil = std::make_shared<LispObject>(LispObjectType::Null);
        True = std::make_shared<LispBoolean>(true);
        False = std::make_shared<LispBoolean>(false);
        Eof = std::make_shared<LispObject>(LispObjectType::Eof);
    }

    LispObjectPtr LispInterpreter::MakeNumber(double val)
    {
        return std::make_shared<LispNumber>(val);
    }

    LispObjectPtr LispInterpreter::MakeSymbol(const std::string &name, LispObjectPtr value)
    {
        return std::make_shared<LispSymbol>(name, value);
    }

    LispObjectPtr LispInterpreter::Cons(LispObjectPtr car, LispObjectPtr cdr)
    {
        return std::make_shared<LispCons>(car, cdr);
    }

    LispObjectPtr LispInterpreter::MakeCFunction(lisp_CFunction cfun, void *data)
    {
        return std::make_shared<LispCFunction>(cfun, data);
    }

    LispObjectPtr LispInterpreter::MakeString(const std::string &str)
    {
        return std::make_shared<LispString>(str);
    }

    LispObjectType LispInterpreter::TypeOf(LispObjectPtr obj)
    {
        return obj->Type;
    }

    bool LispInterpreter::Null(LispObjectPtr obj)
    {
        return obj->Type == LispObjectType::Null;
    }

    bool LispInterpreter::IsEof(LispObjectPtr obj)
    {
        return obj->Type == LispObjectType::Eof;
    }

    double LispInterpreter::NumVal(LispObjectPtr obj)
    {
        if (obj->Type == LispObjectType::Number) {
            return static_cast<LispNumber*>(obj.get())->val_;
        }
        else {
            return std::numeric_limits<double>::quiet_NaN();
        }
    }

    std::string LispInterpreter::SymbolName(LispObjectPtr symbol)
    {
        if (symbol->Type == LispObjectType::Symbol) {
            return static_cast<LispSymbol*>(symbol.get())->name_;
        }
        else {
            // TODO or throw bad type exception?
            return "";
        }
    }

    LispObjectPtr LispInterpreter::SymbolValue(LispObjectPtr symbol)
    {
        if (symbol->Type == LispObjectType::Symbol) {
            return static_cast<LispSymbol*>(symbol.get())->value_;
        }
        else {
            // TODO or throw bad type exception?
            return Nil;
        }
    }

    LispObjectPtr LispInterpreter::SetSymbolValue(LispObjectPtr symbol, LispObjectPtr value)
    {
        if (symbol->Type == LispObjectType::Symbol) {
            static_cast<LispSymbol*>(symbol.get())->value_ = value;
            return value;
        }
        else {
            // TODO or throw bad type exception?
            return Nil;
        }
    }

    LispObjectPtr LispInterpreter::Car(LispObjectPtr obj)
    {
        if (obj->Type == LispObjectType::Cons) {
            return static_cast<LispCons*>(obj.get())->car_;
        }
        else {
            // TODO or throw bad type exception?
            return Nil;
        }
    }

    LispObjectPtr LispInterpreter::Cdr(LispObjectPtr obj)
    {
        if (obj->Type == LispObjectType::Cons) {
            return static_cast<LispCons*>(obj.get())->cdr_;
        }
        else {
            // TODO or throw bad type exception?
            return Nil;
        }
    }

    bool LispInterpreter::BoolVal(LispObjectPtr obj)
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

    std::string LispInterpreter::StringVal(LispObjectPtr obj)
    {
        if (obj->Type == LispObjectType::String) {
            return static_cast<LispString*>(obj.get())->str_;
        }
        else {
            // TODO or throw bad type exception?
            return "";
        }
    }

    bool LispInterpreter::Eq(LispObjectPtr x, LispObjectPtr y)
    {
        if (x->Type == LispObjectType::Number && y->Type == LispObjectType::Number) {
            return static_cast<LispNumber*>(x.get())->val_ == static_cast<LispNumber*>(y.get())->val_;
        } else if (x->Type == LispObjectType::String && y->Type == LispObjectType::String) {
            LispString *stringX = static_cast<LispString*>(x.get());
            LispString *stringY = static_cast<LispString*>(y.get());
            if (stringX->strHashValue_ != stringY->strHashValue_) {
                return false;
            }
            else {
                return stringX->str_ == stringY->str_;
            }
        }
        else {
            return x.get() == y.get();
        }
    }

    LispObjectPtr LispInterpreter::Rplaca(LispObjectPtr cons, LispObjectPtr obj)
    {
        if (cons->Type == LispObjectType::Cons) {
            static_cast<LispCons*>(cons.get())->car_ = obj;
        }
        // TODO if not LispObjectType::Cons?
        return cons;
    }

    LispObjectPtr LispInterpreter::Rplacd(LispObjectPtr cons, LispObjectPtr obj)
    {
        if (cons->Type == LispObjectType::Cons) {
            static_cast<LispCons*>(cons.get())->cdr_ = obj;
        }
        // TODO if not LispObjectType::Cons?
        return cons;
    }

    LispObjectPtr LispInterpreter::ApplyCFunction(LispObjectPtr cfun, LispObjectPtr args, LispObjectPtr env)
    {
        if (cfun->Type == LispObjectType::CFunction) {
            LispCFunction *f = static_cast<LispCFunction*>(cfun.get());
            return f->cfun_(this, args, env, f->data_);
        }
        else {
            // TODO or throw bad type exception?
            return Nil;
        }
    }

}
