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
        LispNumber(double val) : LispObject(LispObjectType::Number), val_(val) { }
        double val_;
    };

    class LispSymbol : public LispObject {
    public:
        LispSymbol(const std::string &str) : LispObject(LispObjectType::Symbol), str_(str) { }
        std::string str_;
    };

    class LispCons : public LispObject {
    public:
        LispCons(LispObjectPtr car, LispObjectPtr cdr) :
            LispObject(LispObjectType::Cons), car_(car), cdr_(cdr) { }
        LispObjectPtr car_;
        LispObjectPtr cdr_;
    };

    void LispInterpreter::InitNil()
    {
        Nil = std::make_shared<LispObject>(LispObjectType::Nil);
    }

    LispObjectPtr LispInterpreter::MakeNumber(double val)
    {
        return std::make_shared<LispNumber>(val);
    }

    LispObjectPtr LispInterpreter::MakeSymbol(const std::string &str)
    {
        return std::make_shared<LispSymbol>(str);
    }

    LispObjectPtr LispInterpreter::Cons(LispObjectPtr car, LispObjectPtr cdr)
    {
        return std::make_shared<LispCons>(car, cdr);
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

    std::string LispInterpreter::StringVal(LispObjectPtr obj)
    {
        if (obj->Type == LispObjectType::Symbol) {
            return static_cast<LispSymbol*>(obj.get())->str_;
        }
        // TODO or throw bad type exception?
        return "";
    }

    LispObjectPtr LispInterpreter::Car(LispObjectPtr obj)
    {
        if (obj->Type == LispObjectType::Cons) {
            return static_cast<LispCons*>(obj.get())->car_;
        }
        // TODO or throw bad type exception?
        return LispInterpreter::Nil;
    }

    LispObjectPtr LispInterpreter::Cdr(LispObjectPtr obj)
    {
        if (obj->Type == LispObjectType::Cons) {
            return static_cast<LispCons*>(obj.get())->cdr_;
        }
        // TODO or throw bad type exception?
        return LispInterpreter::Nil;
    }

    bool LispInterpreter::Eq(LispObjectPtr x, LispObjectPtr y)
    {
        if (x->Type == LispObjectType::Number && y->Type == LispObjectType::Number) {
            return static_cast<LispNumber*>(x.get())->val_ == static_cast<LispNumber*>(y.get())->val_;
        }
        return x.get() == y.get();
    }

}
