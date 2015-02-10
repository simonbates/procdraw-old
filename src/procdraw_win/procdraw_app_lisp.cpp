#include "stdafx.h"
#include "procdraw_app_lisp.h"

// TODO check number and types of arguments in Lisp wrapper functions?

namespace procdraw {

    static ProcDrawApp *app;

    static LispObjectPtr lisp_FramesPerSecond(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env)
    {
        return L->MakeNumber(app->FramesPerSecond());
    }

    static LispObjectPtr lisp_Height(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env)
    {
        return L->MakeNumber(app->Height());
    }

    static LispObjectPtr lisp_MouseX(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env)
    {
        return L->MakeNumber(app->MouseX());
    }

    static LispObjectPtr lisp_MouseY(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env)
    {
        return L->MakeNumber(app->MouseY());
    }

    static LispObjectPtr lisp_Width(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env)
    {
        return L->MakeNumber(app->Width());
    }

    // Graphics

    static LispObjectPtr lisp_AmbientLightColor(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env)
    {
        auto h = L->NumVal(L->Car(args));
        auto s = L->NumVal(L->Cadr(args));
        auto v = L->NumVal(L->Caddr(args));
        app->Graphics()->AmbientLightColor(h, s, v);
        return L->Nil;
    }

    static LispObjectPtr lisp_Background(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env)
    {
        auto h = L->NumVal(L->Car(args));
        auto s = L->NumVal(L->Cadr(args));
        auto v = L->NumVal(L->Caddr(args));
        app->Graphics()->Background(h, s, v);
        return L->Nil;
    }

    static LispObjectPtr lisp_Color(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env)
    {
        auto h = L->NumVal(L->Car(args));
        auto s = L->NumVal(L->Cadr(args));
        auto v = L->NumVal(L->Caddr(args));
        app->Graphics()->Color(h, s, v);
        return L->Nil;
    }

    static LispObjectPtr lisp_LightColor(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env)
    {
        auto h = L->NumVal(L->Car(args));
        auto s = L->NumVal(L->Cadr(args));
        auto v = L->NumVal(L->Caddr(args));
        app->Graphics()->LightColor(h, s, v);
        return L->Nil;
    }

    static LispObjectPtr lisp_RotateX(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env)
    {
        auto angle = L->NumVal(L->Car(args));
        app->Graphics()->RotateX(angle);
        return L->Nil;
    }

    static LispObjectPtr lisp_RotateY(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env)
    {
        auto angle = L->NumVal(L->Car(args));
        app->Graphics()->RotateY(angle);
        return L->Nil;
    }

    static LispObjectPtr lisp_RotateZ(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env)
    {
        auto angle = L->NumVal(L->Car(args));
        app->Graphics()->RotateZ(angle);
        return L->Nil;
    }

    static LispObjectPtr lisp_Scale(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env)
    {
        auto x = L->NumVal(L->Car(args));
        auto y = L->NumVal(L->Cadr(args));
        auto z = L->NumVal(L->Caddr(args));
        app->Graphics()->Scale(x, y, z);
        return L->Nil;
    }

    static LispObjectPtr lisp_Tetrahedron(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env)
    {
        app->Graphics()->Tetrahedron();
        return L->Nil;
    }

    static LispObjectPtr lisp_Translate(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env)
    {
        auto x = L->NumVal(L->Car(args));
        auto y = L->NumVal(L->Cadr(args));
        auto z = L->NumVal(L->Caddr(args));
        app->Graphics()->Translate(x, y, z);
        return L->Nil;
    }

    void RegisterProcDrawAppFunctionsForLisp(ProcDrawApp *a, LispInterpreter *L)
    {
        app = a;
        L->SetGlobalCFunction("frames-per-second", lisp_FramesPerSecond);
        L->SetGlobalCFunction("height", lisp_Height);
        L->SetGlobalCFunction("mouse-x", lisp_MouseX);
        L->SetGlobalCFunction("mouse-y", lisp_MouseY);
        L->SetGlobalCFunction("width", lisp_Width);
        // Graphics
        L->SetGlobalCFunction("ambient-light-color", lisp_AmbientLightColor);
        L->SetGlobalCFunction("background", lisp_Background);
        L->SetGlobalCFunction("color", lisp_Color);
        L->SetGlobalCFunction("light-color", lisp_LightColor);
        L->SetGlobalCFunction("rotate-x", lisp_RotateX);
        L->SetGlobalCFunction("rotate-y", lisp_RotateY);
        L->SetGlobalCFunction("rotate-z", lisp_RotateZ);
        L->SetGlobalCFunction("scale", lisp_Scale);
        L->SetGlobalCFunction("tetrahedron", lisp_Tetrahedron);
        L->SetGlobalCFunction("translate", lisp_Translate);
    }

}
