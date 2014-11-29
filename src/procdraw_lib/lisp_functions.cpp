#include "lisp_functions.h"

#define FOR_EACH(L, n, lst) \
    for (LispObjectPtr n = lst; !L->Null(n); n = L->Cdr(n))

#define REDUCE_NUM(L, acc, x, exp, lst) \
    FOR_EACH(L, REDUCE_NUM_n, lst) { \
        auto x = L->NumVal(L->Car(REDUCE_NUM_n)); \
        acc = exp; \
    }

namespace procdraw {

    LispObjectPtr lisp_Apply(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env)
    {
        // TODO check number of args?
        return L->Apply(L->Car(args), L->Cadr(args), env);
    }

    LispObjectPtr lisp_Difference(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env)
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

    LispObjectPtr lisp_Product(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env)
    {
        double product = 1;
        REDUCE_NUM(L, product, x, product * x, args);
        return L->MakeNumber(product);
    }

    LispObjectPtr lisp_Quotient(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env)
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

    LispObjectPtr lisp_Sum(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env)
    {
        double sum = 0;
        REDUCE_NUM(L, sum, x, sum + x, args);
        return L->MakeNumber(sum);
    }

}
