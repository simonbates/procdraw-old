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

    static LispObjectPtr lisp_Width(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env)
    {
        return(L->MakeNumber(app->Width()));
    }

    // Graphics

    static LispObjectPtr lisp_Background(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env)
    {
        // TODO check number and type of args
        auto h = L->Car(args);
        auto s = L->Cadr(args);
        auto v = L->Caddr(args);
        app->Graphics()->Background(L->NumVal(h), L->NumVal(s), L->NumVal(v));
        return L->Nil;
    }

    static LispObjectPtr lisp_Rotate(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env)
    {
        // TODO check number and type of args
        auto angle = L->Car(args);
        app->Graphics()->Rotate(L->NumVal(angle));
        return L->Nil;
    }

    static LispObjectPtr lisp_Triangle(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env)
    {
        app->Graphics()->Triangle();
        return L->Nil;
    }

    void RegisterProcDrawAppFunctionsForLisp(ProcDrawApp *a, LispInterpreter *L)
    {
        app = a;
        L->SetGlobalCFunction("mousex", lisp_MouseX);
        L->SetGlobalCFunction("mousey", lisp_MouseY);
        L->SetGlobalCFunction("background", lisp_Background);
        L->SetGlobalCFunction("rotate", lisp_Rotate);
        L->SetGlobalCFunction("triangle", lisp_Triangle);
        L->SetGlobalCFunction("width", lisp_Width);
    }

}
