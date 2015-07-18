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

    static LispObjectPtr lisp_Color(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data)
    {
        auto app = static_cast<ProcdrawApp*>(data);
        auto h = L->NumVal(L->Car(args));
        auto s = L->NumVal(L->Cadr(args));
        auto v = L->NumVal(L->Caddr(args));
        app->Renderer()->Color(h, s, v);
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

    static LispObjectPtr lisp_RotateX(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data)
    {
        auto app = static_cast<ProcdrawApp*>(data);
        app->Renderer()->RotateX(L->NumVal(L->Car(args)));
        return L->Nil;
    }

    static LispObjectPtr lisp_RotateY(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data)
    {
        auto app = static_cast<ProcdrawApp*>(data);
        app->Renderer()->RotateY(L->NumVal(L->Car(args)));
        return L->Nil;
    }

    static LispObjectPtr lisp_RotateZ(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data)
    {
        auto app = static_cast<ProcdrawApp*>(data);
        app->Renderer()->RotateZ(L->NumVal(L->Car(args)));
        return L->Nil;
    }

    static LispObjectPtr lisp_Tetrahedron(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data)
    {
        auto app = static_cast<ProcdrawApp*>(data);
        app->Renderer()->Tetrahedron();
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
        L->SetGlobalCFunction("color", lisp_Color, app);
        L->SetGlobalCFunction("frames-per-second", lisp_FramesPerSecond, app);
        L->SetGlobalCFunction("height", lisp_Height, app);
        L->SetGlobalCFunction("mouse-x", lisp_MouseX, app);
        L->SetGlobalCFunction("mouse-y", lisp_MouseY, app);
        L->SetGlobalCFunction("rotate-x", lisp_RotateX, app);
        L->SetGlobalCFunction("rotate-y", lisp_RotateY, app);
        L->SetGlobalCFunction("rotate-z", lisp_RotateZ, app);
        L->SetGlobalCFunction("tetrahedron", lisp_Tetrahedron, app);
        L->SetGlobalCFunction("width", lisp_Width, app);
    }

}
