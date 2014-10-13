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

    if (AttachConsole(ATTACH_PARENT_PROCESS)) {
        FILE *pCout;
        freopen_s(&pCout, "CONOUT$", "w", stdout);
        FILE *pCerr;
        freopen_s(&pCerr, "CONOUT$", "w", stderr);
    }

    procdraw::ProcDrawApp app(hInstance, nCmdShow);

    auto before = GetTickCount();
    int exitCode = app.MainLoop();
    auto after = GetTickCount();

    auto runTime = after - before;
    double fps = (1000.0 * app.FrameCount()) / runTime;
    std::cout << "Run time: " << runTime / 1000 << " seconds" << std::endl;
    std::cout << "Frames: " << app.FrameCount() << std::endl;
    std::cout << "Frames Per Second: " << fps << std::endl;

    return exitCode;
}
