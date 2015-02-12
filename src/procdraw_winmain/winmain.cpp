#include "stdafx.h"
#include "procdraw_app.h"
#include <iostream>

int CALLBACK WinMain(
    _In_  HINSTANCE hInstance,
    _In_  HINSTANCE hPrevInstance,
    _In_  LPSTR lpCmdLine,
    _In_  int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    procdraw::ProcDrawApp app(hInstance, nCmdShow);
    return app.MainLoop();
}
