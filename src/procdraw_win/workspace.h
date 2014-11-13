#pragma once

#include "command_processor.h"
#include <Windows.h>
#include <string>

namespace procdraw {

    class Workspace
    {
    public:
        Workspace(HINSTANCE hInstance, int nCmdShow,
            const std::wstring &name, bool readonly, CommandProcessor *cmdProcessor);
        static void RegisterWindowClass(HINSTANCE hInstance);
        static LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
        HWND GetHwnd();
        void AddLine(const std::string &text);
    private:
        HINSTANCE hInstance_;
        HWND hwnd_;
        HWND hwndEdit_;
        bool readonly_;
        CommandProcessor *cmdProcessor_;
        void CreateWorkspaceWindow(const std::wstring &name);
        void DoCommand(const std::string &cmd);
    };

}
