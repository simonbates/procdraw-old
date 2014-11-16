#include "lisp_functions.h"

namespace procdraw {

    LispObjectPtr lisp_Add(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env)
    {
        double sum = 0;
        LispObjectPtr n = args;
        while (!L->Null(n)) {
            sum += L->NumVal(L->Car(n));
            n = L->Cdr(n);
        }
        return L->MakeNumber(sum);
    }

    LispObjectPtr lisp_Apply(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env)
    {
        // TODO check number of args
        return L->Apply(L->Car(args), L->Cadr(args), env);
    }

}
