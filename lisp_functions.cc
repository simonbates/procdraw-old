#include "lisp_functions.h"
#include "util.h"

#define FOR_EACH(L, n, lst) \
    for (LispObjectPtr n = lst; !L->Null(n); n = L->Cdr(n))

#define REDUCE_NUM(L, acc, x, exp, lst) \
    FOR_EACH(L, REDUCE_NUM_n, lst) { \
        auto x = L->NumVal(L->Car(REDUCE_NUM_n)); \
        acc = exp; \
    }

// TODO check number and types of arguments in Lisp wrapper functions?

namespace procdraw {

    LispObjectPtr lisp_Apply(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data)
    {
        return L->Apply(L->Car(args), L->Cadr(args), env);
    }

    LispObjectPtr lisp_Assoc(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data)
    {
        return L->Assoc(L->Car(args), L->Cadr(args));
    }

    LispObjectPtr lisp_Car(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data)
    {
        return L->Caar(args);
    }

    LispObjectPtr lisp_Cdr(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data)
    {
        return L->Cdar(args);
    }

    LispObjectPtr lisp_Clear(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data)
    {
        return L->Clear(L->Car(args));
    }

    LispObjectPtr lisp_Difference(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data)
    {
        // TODO difference with 0 args? return a value or complain that not enough args?
        if (L->Null(args)) {
            // 0 args
            return L->MakeNumber(0);
        }

        double difference = L->NumVal(L->Car(args));
        LispObjectPtr n = L->Cdr(args);

        if (L->Null(n)) {
            // 1 arg
            return L->MakeNumber(-difference);
        }

        // 2 or more args
        REDUCE_NUM(L, difference, x, difference - x, n);
        return L->MakeNumber(difference);
    }

    LispObjectPtr lisp_Eq(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data)
    {
        return L->Eq(L->Car(args), L->Cadr(args)) ? L->True : L->False;
    }

    LispObjectPtr lisp_Get(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data)
    {
        auto table = L->Car(args);
        auto key = L->Cadr(args);
        return L->Get(table, key);
    }

    LispObjectPtr lisp_Keys(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data)
    {
        return L->Keys(L->Car(args));
    }

    LispObjectPtr lisp_Lerp(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data)
    {
        auto start = L->NumVal(L->Car(args));
        auto stop = L->NumVal(L->Cadr(args));
        auto val = L->NumVal(L->Caddr(args));
        return L->MakeNumber(Lerp(start, stop, val));
    }

    LispObjectPtr lisp_MakeTable(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data)
    {
        return L->MakeTable();
    }

    LispObjectPtr lisp_MapRange(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data)
    {
        auto start1 = L->NumVal(L->Car(args));
        auto stop1 = L->NumVal(L->Cadr(args));
        auto start2 = L->NumVal(L->Caddr(args));
        auto stop2 = L->NumVal(L->Cadddr(args));
        auto val = L->NumVal(L->Caddddr(args));
        return L->MakeNumber(MapRange(start1, stop1, start2, stop2, val));
    }

    LispObjectPtr lisp_Norm(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data)
    {
        auto start = L->NumVal(L->Car(args));
        auto stop = L->NumVal(L->Cadr(args));
        auto val = L->NumVal(L->Caddr(args));
        return L->MakeNumber(Norm(start, stop, val));
    }

    LispObjectPtr lisp_Not(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data)
    {
        return L->Not(L->Car(args));
    }

    LispObjectPtr lisp_Product(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data)
    {
        double product = 1;
        REDUCE_NUM(L, product, x, product * x, args);
        return L->MakeNumber(product);
    }

    LispObjectPtr lisp_Put(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data)
    {
        auto table = L->Car(args);
        auto key = L->Cadr(args);
        auto val = L->Caddr(args);
        return L->Put(table, key, val);
    }

    LispObjectPtr lisp_Putassoc(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data)
    {
        return L->Putassoc(L->Car(args), L->Cadr(args), L->Caddr(args));
    }

    LispObjectPtr lisp_Quotient(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data)
    {
        // TODO quotient with 0 args? return a value or complain that not enough args?
        if (L->Null(args)) {
            // 0 args
            return L->MakeNumber(1);
        }

        double quotient = L->NumVal(L->Car(args));
        LispObjectPtr n = L->Cdr(args);

        if (L->Null(n)) {
            // 1 arg
            return L->MakeNumber(1.0 / quotient);
        }

        // 2 or more args
        REDUCE_NUM(L, quotient, x, quotient / x, n);
        return L->MakeNumber(quotient);
    }

    LispObjectPtr lisp_Sum(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data)
    {
        double sum = 0;
        REDUCE_NUM(L, sum, x, sum + x, args);
        return L->MakeNumber(sum);
    }

    LispObjectPtr lisp_Wrap(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data)
    {
        auto start = L->NumVal(L->Car(args));
        auto stop = L->NumVal(L->Cadr(args));
        auto val = L->NumVal(L->Caddr(args));
        return L->MakeNumber(Wrap(start, stop, val));
    }

}
