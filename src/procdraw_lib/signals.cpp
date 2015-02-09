#include "signals.h"
#include "util.h"

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
            L->ApplyTableMethod(L->SymbolRef("step"), signal, L->Nil, env);
            L->Put(steppedSignals, signal, L->True);
        }

        return L->Get(signal, L->SymbolRef("val1"));
    }

    static LispObjectPtr lisp_ClearSteppedSignals(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env)
    {
        auto steppedSignals = GetSteppedSignals(L);
        return L->Clear(steppedSignals);
    }

    static LispObjectPtr lisp_StepSaw(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env)
    {
        auto self = L->Car(args);
        auto val1Key = L->SymbolRef("val1");
        double val1 = L->NumVal(L->Get(self, val1Key));
        val1 = Wrap(0.0, 1.0, val1 + L->NumVal(L->Get(self, L->SymbolRef("freq"))));
        auto val1Num = L->MakeNumber(val1);
        L->Put(self, val1Key, val1Num);
        return val1Num;
    }

    static LispObjectPtr lisp_Saw(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env)
    {
        auto saw = MakeSignal(L, L->MakeCFunction(lisp_StepSaw));
        L->Put(saw, L->SymbolRef("freq"), L->MakeNumber(0));
        L->Put(saw, L->SymbolRef("val1"), L->MakeNumber(0));
        return saw;
    }

    void RegisterSignals(LispInterpreter *L)
    {
        L->Set(L->SymbolRef("stepped-signals"), L->MakeTable(), L->Nil);
        L->SetGlobalCFunction("make-signal", lisp_MakeSignal);
        L->SetGlobalCFunction("sigval", lisp_Sigval);
        L->SetGlobalCFunction("clear-stepped-signals", lisp_ClearSteppedSignals);
        L->SetGlobalCFunction("saw", lisp_Saw);
    }

}
