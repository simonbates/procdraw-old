#pragma once

#include <memory>
#include <string>

namespace procdraw {

    class LispObject;
    typedef std::shared_ptr<LispObject> LispObjectPtr;

    enum class LispObjectType { Nil, Number, Symbol, Cell };

    class LispMemory {
    public:
        LispMemory() : symbolTable_(Nil) { }
        LispObjectPtr MakeNumber(double val);
        LispObjectPtr MakeSymbol(const std::string &str);
        LispObjectPtr Cons(LispObjectPtr car, LispObjectPtr cdr);
        // Shared Nil instance
        static LispObjectPtr Nil;
        // Object access
        LispObjectType GetType(LispObjectPtr obj);
        bool Null(LispObjectPtr obj);
        double NumVal(LispObjectPtr obj);
        std::string StringVal(LispObjectPtr obj);
        LispObjectPtr Car(LispObjectPtr obj);
        LispObjectPtr Cdr(LispObjectPtr obj);
    private:
        LispObjectPtr symbolTable_;
    };

}
