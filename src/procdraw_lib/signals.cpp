#include "signals.h"

namespace procdraw {

    static LispObjectPtr MakeSignal(LispInterpreter *L, LispObjectPtr stepFun)
    {
        auto signal = L->MakeTable();
        L->Put(signal, L->SymbolRef("step"), stepFun);
        return signal;
    }

    static LispObjectPtr lisp_MakeSignal(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env)
    {
        auto stepFun = L->Car(args);
        return MakeSignal(L, stepFun);
    }

    void RegisterSignals(LispInterpreter *L)
    {
        L->SetGlobalCFunction("make-signal", lisp_MakeSignal);
    }

}
