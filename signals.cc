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

    static LispObjectPtr PutassocHolder(LispInterpreter *L, LispObjectPtr key,
                                        LispObjectPtr val, LispObjectPtr holder)
    {
        auto alist = L->Car(holder);
        if (L->Null(alist)) {
            L->Rplaca(holder, L->Cons(L->Cons(key, val), L->Nil));
        }
        else {
            L->Putassoc(key, val, alist);
        }
        return val;
    }

    static LispObjectPtr SlotsHolder(LispInterpreter *L, LispObjectPtr signal)
    {
        return L->Cdr(signal);
    }

    static LispObjectPtr InputsHolder(LispInterpreter *L, LispObjectPtr signal)
    {
        return L->Cddr(signal);
    }

    LispObjectPtr GetSlot(LispInterpreter *L, LispObjectPtr signal, LispObjectPtr key)
    {
        return L->Cdr(L->Assoc(key, L->Car(SlotsHolder(L, signal))));
    }

    LispObjectPtr PutSlot(LispInterpreter *L, LispObjectPtr signal,
                          LispObjectPtr key, LispObjectPtr val)
    {
        return PutassocHolder(L, key, val, SlotsHolder(L, signal));
    }

    LispObjectPtr MakeSignal(LispInterpreter *L, LispObjectPtr stepFun)
    {
        auto signal = L->MakeList({ L->SymbolRef("signal"), L->Nil, L->Nil });
        PutSlot(L, signal, L->SymbolRef("step"), stepFun);
        return signal;
    }

    static LispObjectPtr lisp_StepEventRelay(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data)
    {
        auto self = L->Car(args);
        auto eventKey = L->SymbolRef("event");
        auto event = GetSlot(L, self, eventKey);
        PutSlot(L, self, L->SymbolRef("val"), event);
        PutSlot(L, self, eventKey, L->Nil);
        return event;
    }

    LispObjectPtr MakeEventRelaySignal(LispInterpreter *L)
    {
        return MakeSignal(L, L->MakeCFunction(lisp_StepEventRelay, nullptr));
    }

    static bool Signalp(LispInterpreter *L, LispObjectPtr obj)
    {
        return L->TypeOf(obj) == LispObjectType::Cons && L->Eq(L->Car(obj), L->SymbolRef("signal"));
    }

    static bool HasBeenStepped(LispInterpreter *L, LispObjectPtr signal)
    {
        return L->Memb(signal, L->SymbolValue(L->SymbolRef("stepped-signals")));
    }

    static void RecordAsStepped(LispInterpreter *L, LispObjectPtr signal)
    {
        L->Set(L->SymbolRef("stepped-signals"),
               L->Cons(signal, L->SymbolValue(L->SymbolRef("stepped-signals"))),
               L->Nil);
    }

    static LispObjectPtr Sigval(LispInterpreter *L, LispObjectPtr signal, LispObjectPtr env)
    {
        auto stepFun = GetSlot(L, signal, L->SymbolRef("step"));
        if (!L->Null(stepFun) && !HasBeenStepped(L, signal)) {
            // Apply inputs
            for (LispObjectPtr n = L->Car(InputsHolder(L, signal)); !L->Null(n); n = L->Cdr(n)) {
                auto key = L->Caar(n);
                auto sourceSpec = L->Cdar(n);
                auto source = L->Car(sourceSpec);
                auto mapFun = L->Cdr(sourceSpec);

                auto sourceVal = Sigval(L, source, env);

                if (L->Null(mapFun)) {
                    PutSlot(L, signal, key, sourceVal);
                }
                else {
                    PutSlot(L, signal, key, L->Apply(mapFun, L->Cons(sourceVal, L->Nil), env));
                }
            }
            // Step
            L->Apply(stepFun, L->Cons(signal, L->Nil), env);
            RecordAsStepped(L, signal);
        }

        return GetSlot(L, signal, L->SymbolRef("val"));
    }

    static LispObjectPtr MakeWavetableOscillator(LispInterpreter *L, LispObjectPtr stepFun)
    {
        auto sig = MakeSignal(L, stepFun);
        PutSlot(L, sig, L->SymbolRef("freq"), L->MakeNumber(0));
        PutSlot(L, sig, L->SymbolRef("index"), L->MakeNumber(0));
        PutSlot(L, sig, L->SymbolRef("val"), L->MakeNumber(0));
        return sig;
    }

    static LispObjectPtr StepWavetableOscillator(LispInterpreter *L, LispObjectPtr signal, double *wavetable,
            int wavetableLen)
    {
        int wavetableEnd = wavetableLen - 1;
        double incr = L->NumVal(GetSlot(L, signal, L->SymbolRef("freq"))) * wavetableEnd;
        auto indexKey = L->SymbolRef("index");
        double index = L->NumVal(GetSlot(L, signal, indexKey));
        index = Wrap(0.0, wavetableEnd, index + incr);
        if (index >= wavetableEnd || index < 0.0) {
            index = 0.0;
        }
        PutSlot(L, signal, indexKey, L->MakeNumber(index));
        int indexBefore = floor(index);
        double val = Lerp(wavetable[indexBefore], wavetable[indexBefore + 1], index - indexBefore);
        auto valNum = L->MakeNumber(val);
        PutSlot(L, signal, L->SymbolRef("val"), valNum);
        return valNum;
    }

    static LispObjectPtr lisp_MakeSignal(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data)
    {
        return MakeSignal(L, L->Car(args));
    }

    static LispObjectPtr lisp_GetSlot(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data)
    {
        return GetSlot(L, L->Car(args), L->Cadr(args));
    }

    static LispObjectPtr lisp_PutSlot(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data)
    {
        return PutSlot(L, L->Car(args), L->Cadr(args), L->Caddr(args));
    }

    static LispObjectPtr lisp_Connect(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data)
    {
        auto source = L->Car(args);
        auto destSignal = L->Cadr(args);
        auto destKey = L->Caddr(args);
        auto mapFun = L->Cadddr(args);

        return PutassocHolder(L, destKey, L->Cons(source, mapFun), InputsHolder(L, destSignal));
    }

    static LispObjectPtr lisp_Sigval(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data)
    {
        return Sigval(L, L->Car(args), env);
    }

    static LispObjectPtr lisp_ClearSteppedSignals(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data)
    {
        return L->Set(L->SymbolRef("stepped-signals"), L->Nil, L->Nil);
    }

    static LispObjectPtr lisp_StepSaw(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data)
    {
        auto self = L->Car(args);
        auto valKey = L->SymbolRef("val");
        double val = L->NumVal(GetSlot(L, self, valKey));
        val = Wrap(0.0, 1.0, val + L->NumVal(GetSlot(L, self, L->SymbolRef("freq"))));
        auto valNum = L->MakeNumber(val);
        PutSlot(L, self, valKey, valNum);
        return valNum;
    }

    static LispObjectPtr lisp_Saw(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data)
    {
        auto saw = MakeSignal(L, L->MakeCFunction(lisp_StepSaw, nullptr));
        PutSlot(L, saw, L->SymbolRef("freq"), L->MakeNumber(0));
        PutSlot(L, saw, L->SymbolRef("val"), L->MakeNumber(0));
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
        L->Set(L->SymbolRef("stepped-signals"), L->Nil, L->Nil);
        L->SetGlobalCFunction("make-signal", lisp_MakeSignal, nullptr);
        L->SetGlobalCFunction("get-slot", lisp_GetSlot, nullptr);
        L->SetGlobalCFunction("put-slot", lisp_PutSlot, nullptr);
        L->SetGlobalCFunction("=>", lisp_Connect, nullptr);
        L->SetGlobalCFunction("sigval", lisp_Sigval, nullptr);
        L->SetGlobalCFunction("clear-stepped-signals", lisp_ClearSteppedSignals, nullptr);
        L->SetGlobalCFunction("saw", lisp_Saw, nullptr);
        L->SetGlobalCFunction("sin-osc", lisp_SinOsc, nullptr);
        L->SetGlobalCFunction("tri", lisp_Tri, nullptr);
    }

}
