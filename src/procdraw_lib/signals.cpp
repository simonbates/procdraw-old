#include "signals.h"

namespace procdraw {

    static LispObjectPtr MakeSignal(LispInterpreter *L, LispObjectPtr stepFun)
    {
        auto signal = L->MakeTable();
        L->Put(signal, L->SymbolRef("step"), stepFun);
        return signal;
    }

    static LispObjectPtr GetSteppedSignals(LispInterpreter *L)
    {
        return L->SymbolValue(L->SymbolRef("stepped-signals"));
    }

    static LispObjectPtr lisp_MakeSignal(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env)
    {
        return MakeSignal(L, L->Car(args));
    }

    static LispObjectPtr lisp_Sigval(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env)
    {
        auto signal = L->Car(args);
        auto steppedSignals = GetSteppedSignals(L);

        if (L->Null(L->Get(steppedSignals, signal))) {
            L->Apply(L->SymbolRef("step"), L->Cons(signal, L->Nil), env);
            L->Put(steppedSignals, signal, L->True);
        }

        return L->Get(signal, L->SymbolRef("val1"));
    }

    static LispObjectPtr lisp_ClearSteppedSignals(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env)
    {
        auto steppedSignals = GetSteppedSignals(L);
        return L->Clear(steppedSignals);
    }

    void RegisterSignals(LispInterpreter *L)
    {
        L->Set(L->SymbolRef("stepped-signals"), L->MakeTable(), L->Nil);
        L->SetGlobalCFunction("make-signal", lisp_MakeSignal);
        L->SetGlobalCFunction("sigval", lisp_Sigval);
        L->SetGlobalCFunction("clear-stepped-signals", lisp_ClearSteppedSignals);
    }

}
