#include "lisp_functions.h"

namespace procdraw {

    LispObjectPtr lisp_Apply(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env)
    {
        // TODO check number of args
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
        while (!L->Null(n)) {
            difference -= L->NumVal(L->Car(n));
            n = L->Cdr(n);
        }
        return L->MakeNumber(difference);
    }

    LispObjectPtr lisp_Product(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env)
    {
        double product = 1;
        LispObjectPtr n = args;
        while (!L->Null(n)) {
            product *= L->NumVal(L->Car(n));
            n = L->Cdr(n);
        }
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
        while (!L->Null(n)) {
            quotient /= L->NumVal(L->Car(n));
            n = L->Cdr(n);
        }
        return L->MakeNumber(quotient);
    }

    LispObjectPtr lisp_Sum(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env)
    {
        double sum = 0;
        LispObjectPtr n = args;
        while (!L->Null(n)) {
            sum += L->NumVal(L->Car(n));
            n = L->Cdr(n);
        }
        return L->MakeNumber(sum);
    }

}
