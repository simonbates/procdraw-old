#include "procdraw_app_sdl_lisp.h"
#include "globals.h"

namespace procdraw {

    static LispObjectPtr lisp_Background(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env)
    {
        auto h = L->NumVal(L->Car(args));
        auto s = L->NumVal(L->Cadr(args));
        auto v = L->NumVal(L->Caddr(args));
        pd_app->Renderer()->Background(h, s, v);
        return L->Nil;
    }

    static LispObjectPtr lisp_Colour(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env)
    {
        auto h = L->NumVal(L->Car(args));
        auto s = L->NumVal(L->Cadr(args));
        auto v = L->NumVal(L->Caddr(args));
        pd_app->Renderer()->Colour(h, s, v);
        return L->Nil;
    }

    static LispObjectPtr lisp_Height(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env)
    {
        return L->MakeNumber(pd_app->Renderer()->Height());
    }

    static LispObjectPtr lisp_MouseX(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env)
    {
        return L->MakeNumber(pd_app->Renderer()->MouseX());
    }

    static LispObjectPtr lisp_MouseY(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env)
    {
        return L->MakeNumber(pd_app->Renderer()->MouseY());
    }

    static LispObjectPtr lisp_Point(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env)
    {
        pd_app->Renderer()->Point();
        return L->Nil;
    }

    static LispObjectPtr lisp_RotateZ(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env)
    {
        pd_app->Renderer()->RotateZ(L->NumVal(L->Car(args)));
        return L->Nil;
    }

    static LispObjectPtr lisp_Triangle(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env)
    {
        pd_app->Renderer()->Triangle();
        return L->Nil;
    }

    static LispObjectPtr lisp_Width(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env)
    {
        return L->MakeNumber(pd_app->Renderer()->Width());
    }

    void RegisterProcDrawAppSdlFunctions(LispInterpreter *L)
    {
        L->SetGlobalCFunction("background", lisp_Background);
        L->SetGlobalCFunction("colour", lisp_Colour);
        L->SetGlobalCFunction("height", lisp_Height);
        L->SetGlobalCFunction("mouse-x", lisp_MouseX);
        L->SetGlobalCFunction("mouse-y", lisp_MouseY);
        L->SetGlobalCFunction("point", lisp_Point);
        L->SetGlobalCFunction("rotate-z", lisp_RotateZ);
        L->SetGlobalCFunction("triangle", lisp_Triangle);
        L->SetGlobalCFunction("width", lisp_Width);
    }

}
