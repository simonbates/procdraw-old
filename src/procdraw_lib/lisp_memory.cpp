#include "lisp_memory.h"
#include <sstream>

namespace procdraw {

    class LispObject {
    public:
        LispObject(LispObjectType t) : Type(t) { }
        virtual ~LispObject() { }
        const LispObjectType Type;
    };

    class LispNil : public LispObject {
    public:
        LispNil() : LispObject(LispObjectType::Nil) { }
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

    LispObjectPtr LispMemory::MakeNumber(double val)
    {
        return std::make_shared<LispNumber>(val);
    }

    LispObjectPtr LispMemory::MakeSymbol(const std::string &str)
    {
        LispObjectPtr n = symbolTable_;
        while (!Null(n)) {
            auto symbol = Car(n);
            if (StringVal(symbol) == str) {
                return symbol;
            }
            n = Cdr(n);
        }
        auto symbol = std::make_shared<LispSymbol>(str);
        symbolTable_ = Cons(symbol, symbolTable_);
        return symbol;
    }

    LispObjectPtr LispMemory::Cons(LispObjectPtr car, LispObjectPtr cdr)
    {
        return std::make_shared<LispCons>(car, cdr);
    }

    LispObjectPtr LispMemory::Nil = std::make_shared<LispNil>();

    LispObjectType LispMemory::TypeOf(LispObjectPtr obj)
    {
        return obj->Type;
    }

    bool LispMemory::Null(LispObjectPtr obj)
    {
        return obj->Type == LispObjectType::Nil;
    }

    double LispMemory::NumVal(LispObjectPtr obj)
    {
        if (obj->Type == LispObjectType::Number) {
            return static_cast<LispNumber*>(obj.get())->val_;
        }
        // TODO or throw bad type exception?
        return 0.0;
    }

    std::string LispMemory::StringVal(LispObjectPtr obj)
    {
        if (obj->Type == LispObjectType::Symbol) {
            return static_cast<LispSymbol*>(obj.get())->str_;
        }
        // TODO or throw bad type exception?
        return "";
    }

    LispObjectPtr LispMemory::Car(LispObjectPtr obj)
    {
        if (obj->Type == LispObjectType::Cons) {
            return static_cast<LispCons*>(obj.get())->car_;
        }
        // TODO or throw bad type exception?
        return LispMemory::Nil;
    }

    LispObjectPtr LispMemory::Cdr(LispObjectPtr obj)
    {
        if (obj->Type == LispObjectType::Cons) {
            return static_cast<LispCons*>(obj.get())->cdr_;
        }
        // TODO or throw bad type exception?
        return LispMemory::Nil;
    }

}
