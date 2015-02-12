#include "signals.h"
#include "util.h"

namespace procdraw {

    static LispObjectPtr MakeSignal(LispInterpreter *L, LispObjectPtr stepFun)
    {
        auto signal = L->MakeTable();
        L->Put(signal, L->SymbolRef("inputs"), L->MakeTable());
        L->Put(signal, L->SymbolRef("step"), stepFun);
        return signal;
    }

    static LispObjectPtr GetSignalInputs(LispInterpreter *L, LispObjectPtr signal)
    {
        return L->Get(signal, L->SymbolRef("inputs"));
    }

    static LispObjectPtr GetSteppedSignals(LispInterpreter *L)
    {
        return L->SymbolValue(L->SymbolRef("stepped-signals"));
    }

    static LispObjectPtr Sigval(LispInterpreter *L, LispObjectPtr signal,
                                LispObjectPtr steppedSignals, LispObjectPtr env)
    {
        if (L->Null(L->Get(steppedSignals, signal))) {
            // apply inputs
            auto inputs = GetSignalInputs(L, signal);
            for (LispObjectPtr n = L->Keys(inputs); !L->Null(n); n = L->Cdr(n)) {
                auto key = L->Car(n);
                auto sourceSpec = L->Get(inputs, key);
                auto source = L->Car(sourceSpec);
                auto mapFun = L->Cdr(sourceSpec);

                LispObjectPtr sourceVal = L->Nil;

                switch (L->TypeOf(source)) {
                case LispObjectType::Table:
                    sourceVal = Sigval(L, source, steppedSignals, env);
                    break;
                case LispObjectType::Cons:
                case LispObjectType::CFunction:
                    sourceVal = L->Apply(source, L->Nil, env);
                    break;
                }

                if (L->Null(mapFun)) {
                    L->Put(signal, key, sourceVal);
                }
                else {
                    L->Put(signal, key, L->Apply(mapFun, L->Cons(sourceVal, L->Nil), env));
                }
            }
            // step
            L->ApplyTableMethod(L->SymbolRef("step"), signal, L->Nil, env);
            L->Put(steppedSignals, signal, L->True);
        }

        return L->Get(signal, L->SymbolRef("val1"));
    }

    static LispObjectPtr lisp_MakeSignal(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env)
    {
        return MakeSignal(L, L->Car(args));
    }

    static LispObjectPtr lisp_Connect(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env)
    {
        auto source = L->Car(args);
        auto destSignal = L->Cadr(args);
        auto destKey = L->Caddr(args);
        auto mapFun = L->Cadddr(args);

        return L->Put(GetSignalInputs(L, destSignal), destKey, L->Cons(source, mapFun));
    }

    static LispObjectPtr lisp_Sigval(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env)
    {
        return Sigval(L, L->Car(args), GetSteppedSignals(L), env);
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
        L->SetGlobalCFunction("=>", lisp_Connect);
        L->SetGlobalCFunction("sigval", lisp_Sigval);
        L->SetGlobalCFunction("clear-stepped-signals", lisp_ClearSteppedSignals);
        L->SetGlobalCFunction("saw", lisp_Saw);
    }

}
