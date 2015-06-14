#include "signals.h"
#include "util.h"
#include <cmath>

#define TRI_WAVETABLE_LEN 3
#define SIN_WAVETABLE_LEN 1024

namespace procdraw {

    static double triWavetable[TRI_WAVETABLE_LEN];
    static double sinWavetable[SIN_WAVETABLE_LEN];

    static void InitTriWavetable()
    {
        triWavetable[0] = 0;
        triWavetable[1] = 1;
        triWavetable[2] = 0;
    }

    static void InitSinWavetable()
    {
        int wavetableEnd = SIN_WAVETABLE_LEN - 1;
        for (int i = 0; i < SIN_WAVETABLE_LEN; ++i) {
            double x = (static_cast<double>(i) / static_cast<double>(wavetableEnd)) * 2 * M_PI;
            sinWavetable[i] = (sin(x) + 1) / 2;
        }
    }

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

    static LispObjectPtr MakeWavetableOscillator(LispInterpreter *L, LispObjectPtr stepFun)
    {
        auto sig = MakeSignal(L, stepFun);
        L->Put(sig, L->SymbolRef("freq"), L->MakeNumber(0));
        L->Put(sig, L->SymbolRef("index"), L->MakeNumber(0));
        L->Put(sig, L->SymbolRef("val1"), L->MakeNumber(0));
        return sig;
    }

    static LispObjectPtr StepWavetableOscillator(LispInterpreter *L, LispObjectPtr signal, double *wavetable,
            int wavetableLen)
    {
        int wavetableEnd = wavetableLen - 1;
        double incr = L->NumVal(L->Get(signal, L->SymbolRef("freq"))) * wavetableEnd;
        auto indexKey = L->SymbolRef("index");
        double index = L->NumVal(L->Get(signal, indexKey));
        index = Wrap(0.0, wavetableEnd, index + incr);
        if (index >= wavetableEnd || index < 0.0) {
            index = 0.0;
        }
        L->Put(signal, indexKey, L->MakeNumber(index));
        int indexBefore = floor(index);
        double val1 = Lerp(wavetable[indexBefore], wavetable[indexBefore + 1], index - indexBefore);
        auto val1Num = L->MakeNumber(val1);
        L->Put(signal, L->SymbolRef("val1"), val1Num);
        return val1Num;
    }

    static LispObjectPtr lisp_MakeSignal(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data)
    {
        return MakeSignal(L, L->Car(args));
    }

    static LispObjectPtr lisp_Connect(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data)
    {
        auto source = L->Car(args);
        auto destSignal = L->Cadr(args);
        auto destKey = L->Caddr(args);
        auto mapFun = L->Cadddr(args);

        return L->Put(GetSignalInputs(L, destSignal), destKey, L->Cons(source, mapFun));
    }

    static LispObjectPtr lisp_Sigval(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data)
    {
        return Sigval(L, L->Car(args), GetSteppedSignals(L), env);
    }

    static LispObjectPtr lisp_ClearSteppedSignals(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data)
    {
        auto steppedSignals = GetSteppedSignals(L);
        return L->Clear(steppedSignals);
    }

    static LispObjectPtr lisp_StepSaw(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data)
    {
        auto self = L->Car(args);
        auto val1Key = L->SymbolRef("val1");
        double val1 = L->NumVal(L->Get(self, val1Key));
        val1 = Wrap(0.0, 1.0, val1 + L->NumVal(L->Get(self, L->SymbolRef("freq"))));
        auto val1Num = L->MakeNumber(val1);
        L->Put(self, val1Key, val1Num);
        return val1Num;
    }

    static LispObjectPtr lisp_Saw(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data)
    {
        auto saw = MakeSignal(L, L->MakeCFunction(lisp_StepSaw, nullptr));
        L->Put(saw, L->SymbolRef("freq"), L->MakeNumber(0));
        L->Put(saw, L->SymbolRef("val1"), L->MakeNumber(0));
        return saw;
    }

    static LispObjectPtr lisp_StepTri(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data)
    {
        return StepWavetableOscillator(L, L->Car(args), triWavetable, TRI_WAVETABLE_LEN);
    }

    static LispObjectPtr lisp_Tri(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data)
    {
        return MakeWavetableOscillator(L, L->MakeCFunction(lisp_StepTri, nullptr));
    }

    static LispObjectPtr lisp_StepSinOsc(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data)
    {
        return StepWavetableOscillator(L, L->Car(args), sinWavetable, SIN_WAVETABLE_LEN);
    }

    static LispObjectPtr lisp_SinOsc(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data)
    {
        return MakeWavetableOscillator(L, L->MakeCFunction(lisp_StepSinOsc, nullptr));
    }

    void RegisterSignals(LispInterpreter *L)
    {
        InitTriWavetable();
        InitSinWavetable();
        L->Set(L->SymbolRef("stepped-signals"), L->MakeTable(), L->Nil);
        L->SetGlobalCFunction("make-signal", lisp_MakeSignal, nullptr);
        L->SetGlobalCFunction("=>", lisp_Connect, nullptr);
        L->SetGlobalCFunction("sigval", lisp_Sigval, nullptr);
        L->SetGlobalCFunction("clear-stepped-signals", lisp_ClearSteppedSignals, nullptr);
        L->SetGlobalCFunction("saw", lisp_Saw, nullptr);
        L->SetGlobalCFunction("sin-osc", lisp_SinOsc, nullptr);
        L->SetGlobalCFunction("tri", lisp_Tri, nullptr);
    }

}
