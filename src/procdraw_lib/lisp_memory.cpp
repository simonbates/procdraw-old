#include "lisp_memory.h"
#include "lisp_interpreter.h"

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
        LispCFunction(lisp_CFunction cfun) :
            LispObject(LispObjectType::CFunction),
            cfun_(cfun) { }
        lisp_CFunction cfun_;
    };

    void LispInterpreter::InitNil()
    {
        Nil = std::make_shared<LispObject>(LispObjectType::Nil);
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

    LispObjectPtr LispInterpreter::MakeCFunction(lisp_CFunction cfun)
    {
        return std::make_shared<LispCFunction>(cfun);
    }

    LispObjectType LispInterpreter::TypeOf(LispObjectPtr obj)
    {
        return obj->Type;
    }

    bool LispInterpreter::Null(LispObjectPtr obj)
    {
        return obj->Type == LispObjectType::Nil;
    }

    double LispInterpreter::NumVal(LispObjectPtr obj)
    {
        if (obj->Type == LispObjectType::Number) {
            return static_cast<LispNumber*>(obj.get())->val_;
        }
        // TODO or throw bad type exception?
        return 0.0;
    }

    std::string LispInterpreter::SymbolName(LispObjectPtr symbol)
    {
        if (symbol->Type == LispObjectType::Symbol) {
            return static_cast<LispSymbol*>(symbol.get())->name_;
        }
        // TODO or throw bad type exception?
        return "";
    }

    LispObjectPtr LispInterpreter::SymbolValue(LispObjectPtr symbol)
    {
        if (symbol->Type == LispObjectType::Symbol) {
            return static_cast<LispSymbol*>(symbol.get())->value_;
        }
        // TODO or throw bad type exception?
        return Nil;
    }

    LispObjectPtr LispInterpreter::SetSymbolValue(LispObjectPtr symbol, LispObjectPtr value)
    {
        if (symbol->Type == LispObjectType::Symbol) {
            static_cast<LispSymbol*>(symbol.get())->value_ = value;
            return value;
        }
        // TODO or throw bad type exception?
        return Nil;
    }

    LispObjectPtr LispInterpreter::Car(LispObjectPtr obj)
    {
        if (obj->Type == LispObjectType::Cons) {
            return static_cast<LispCons*>(obj.get())->car_;
        }
        // TODO or throw bad type exception?
        return Nil;
    }

    LispObjectPtr LispInterpreter::Cdr(LispObjectPtr obj)
    {
        if (obj->Type == LispObjectType::Cons) {
            return static_cast<LispCons*>(obj.get())->cdr_;
        }
        // TODO or throw bad type exception?
        return Nil;
    }

    bool LispInterpreter::Eq(LispObjectPtr x, LispObjectPtr y)
    {
        if (x->Type == LispObjectType::Number && y->Type == LispObjectType::Number) {
            return static_cast<LispNumber*>(x.get())->val_ == static_cast<LispNumber*>(y.get())->val_;
        }
        else if (x->Type == LispObjectType::Nil && y->Type == LispObjectType::Nil) {
            return true;
        }
        return x.get() == y.get();
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
            return static_cast<LispCFunction*>(cfun.get())->cfun_(this, args, env);
        }
        // TODO or throw bad type exception?
        return Nil;
    }

}
