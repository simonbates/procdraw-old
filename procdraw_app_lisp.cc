#include "procdraw_app_lisp.h"

namespace procdraw {

    static LispObjectPtr lisp_Background(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data)
    {
        auto app = static_cast<ProcdrawApp*>(data);
        auto h = L->NumVal(L->Car(args));
        auto s = L->NumVal(L->Cadr(args));
        auto v = L->NumVal(L->Caddr(args));
        app->Renderer()->Background(h, s, v);
        return L->Nil;
    }

    static LispObjectPtr lisp_Colour(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data)
    {
        auto app = static_cast<ProcdrawApp*>(data);
        auto h = L->NumVal(L->Car(args));
        auto s = L->NumVal(L->Cadr(args));
        auto v = L->NumVal(L->Caddr(args));
        app->Renderer()->Colour(h, s, v);
        return L->Nil;
    }

    static LispObjectPtr lisp_FramesPerSecond(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data)
    {
        auto app = static_cast<ProcdrawApp*>(data);
        return L->MakeNumber(app->FramesPerSecond());
    }

    static LispObjectPtr lisp_Height(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data)
    {
        auto app = static_cast<ProcdrawApp*>(data);
        return L->MakeNumber(app->Renderer()->Height());
    }

    static LispObjectPtr lisp_MouseX(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data)
    {
        auto app = static_cast<ProcdrawApp*>(data);
        return L->MakeNumber(app->Renderer()->MouseX());
    }

    static LispObjectPtr lisp_MouseY(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data)
    {
        auto app = static_cast<ProcdrawApp*>(data);
        return L->MakeNumber(app->Renderer()->MouseY());
    }

    static LispObjectPtr lisp_Point(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data)
    {
        auto app = static_cast<ProcdrawApp*>(data);
        app->Renderer()->Point();
        return L->Nil;
    }

    static LispObjectPtr lisp_RotateZ(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data)
    {
        auto app = static_cast<ProcdrawApp*>(data);
        app->Renderer()->RotateZ(L->NumVal(L->Car(args)));
        return L->Nil;
    }

    static LispObjectPtr lisp_Triangle(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data)
    {
        auto app = static_cast<ProcdrawApp*>(data);
        app->Renderer()->Triangle();
        return L->Nil;
    }

    static LispObjectPtr lisp_Width(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data)
    {
        auto app = static_cast<ProcdrawApp*>(data);
        return L->MakeNumber(app->Renderer()->Width());
    }

    void RegisterProcdrawAppFunctions(ProcdrawApp *app, LispInterpreter *L)
    {
        L->SetGlobalCFunction("background", lisp_Background, app);
        L->SetGlobalCFunction("colour", lisp_Colour, app);
        L->SetGlobalCFunction("frames-per-second", lisp_FramesPerSecond, app);
        L->SetGlobalCFunction("height", lisp_Height, app);
        L->SetGlobalCFunction("mouse-x", lisp_MouseX, app);
        L->SetGlobalCFunction("mouse-y", lisp_MouseY, app);
        L->SetGlobalCFunction("point", lisp_Point, app);
        L->SetGlobalCFunction("rotate-z", lisp_RotateZ, app);
        L->SetGlobalCFunction("triangle", lisp_Triangle, app);
        L->SetGlobalCFunction("width", lisp_Width, app);
    }

}
