#include "lisp_memory.h"
#include <sstream>

namespace procdraw {

    // LispObject

    class LispObject {
    public:
        virtual ~LispObject() { }
        virtual LispObjectType GetType() = 0;
        virtual bool Null()
        {
            return false;
        }
        virtual double NumVal()
        {
            return 0.0;
        }
        virtual std::string StringVal()
        {
            return "";
        }
        virtual LispObjectPtr Car()
        {
            return LispMemory::Nil;
        }
        virtual LispObjectPtr Cdr()
        {
            return LispMemory::Nil;
        }
    };

    // LispNil

    class LispNil : public LispObject {
    public:
        LispObjectType GetType() override
        {
            return LispObjectType::Nil;
        }
        bool Null() override
        {
            return true;
        }
    };

    // LispNumber

    class LispNumber : public LispObject {
    public:
        LispNumber(double val) : val_(val) { }
        LispObjectType GetType() override
        {
            return LispObjectType::Number;
        }
        double NumVal() override
        {
            return val_;
        }
    private:
        double val_;
    };

    // LispSymbol

    class LispSymbol : public LispObject {
    public:
        LispSymbol(const std::string &str) : str_(str) { }
        LispObjectType GetType() override
        {
            return LispObjectType::Symbol;
        }
        std::string StringVal() override
        {
            return str_;
        }
    private:
        std::string str_;
    };

    // LispCell

    class LispCell : public LispObject {
    public:
        LispCell(LispObjectPtr car, LispObjectPtr cdr) : car_(car), cdr_(cdr) { }
        LispObjectType GetType() override
        {
            return LispObjectType::Cell;
        }
        LispObjectPtr Car() override
        {
            return car_;
        }
        LispObjectPtr Cdr() override
        {
            return cdr_;
        }
    private:
        LispObjectPtr car_;
        LispObjectPtr cdr_;
    };

    // LispMemory

    LispObjectPtr LispMemory::MakeNumber(double val)
    {
        return std::make_shared<LispNumber>(val);
    }

    LispObjectPtr LispMemory::MakeSymbol(const std::string &str)
    {
        LispObjectPtr n = symbolTable_;
        while (!n->Null()) {
            auto symbol = n->Car();
            if (symbol->StringVal() == str) {
                return symbol;
            }
            n = n->Cdr();
        }
        auto symbol = std::make_shared<LispSymbol>(str);
        symbolTable_ = Cons(symbol, symbolTable_);
        return symbol;
    }

    LispObjectPtr LispMemory::Cons(LispObjectPtr car, LispObjectPtr cdr)
    {
        return std::make_shared<LispCell>(car, cdr);
    }

    LispObjectPtr LispMemory::Nil = std::make_shared<LispNil>();

    LispObjectType LispMemory::GetType(LispObjectPtr obj)
    {
        return obj->GetType();
    }

    bool LispMemory::Null(LispObjectPtr obj)
    {
        return obj->Null();
    }

    double LispMemory::NumVal(LispObjectPtr obj)
    {
        return obj->NumVal();
    }

    std::string LispMemory::StringVal(LispObjectPtr obj)
    {
        return obj->StringVal();
    }

    LispObjectPtr LispMemory::Car(LispObjectPtr obj)
    {
        return obj->Car();
    }

    LispObjectPtr LispMemory::Cdr(LispObjectPtr obj)
    {
        return obj->Cdr();
    }

}
