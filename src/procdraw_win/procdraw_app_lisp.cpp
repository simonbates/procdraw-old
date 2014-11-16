#include "stdafx.h"
#include "procdraw_app_lisp.h"

namespace procdraw {

    static ProcDrawApp *app;

    static LispObjectPtr lisp_MouseX(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env)
    {
        return(L->MakeNumber(app->MouseX()));
    }

    static LispObjectPtr lisp_MouseY(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env)
    {
        return(L->MakeNumber(app->MouseY()));
    }

    void RegisterProcDrawAppFunctionsForLisp(ProcDrawApp *a, LispInterpreter *L)
    {
        app = a;
        L->SetGlobalCFunction("mousex", lisp_MouseX);
        L->SetGlobalCFunction("mousey", lisp_MouseY);
    }

}
