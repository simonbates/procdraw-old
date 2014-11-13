#pragma once

#include "command_processor.h"
#include "d3d11graphics.h"
#include "workspace.h"
#include "lisp_interpreter.h"
#include <string>
#include <memory>

namespace procdraw {

    class ProcDrawApp : public CommandProcessor {
    public:
        ProcDrawApp(HINSTANCE hInstance, int nCmdShow);
        inline D3D11Graphics & graphics()
        {
            return *graphics_;
        }
        static LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
        int MainLoop();
        void DoCommand(const std::string &cmd) override;
        long FrameCount();
        float Width();
        float Height();
        float MouseX();
        float MouseY();
    private:
        HINSTANCE hInstance_;
        int nCmdShow_;
        HWND hWnd_;
        HACCEL hAccel_;
        std::shared_ptr<D3D11Graphics> graphics_;
        std::unique_ptr<Workspace> workspace_;
        std::unique_ptr<Workspace> transcript_;
        long frameCount_;
        LispInterpreter L_;
        void CreateAppWindow();
        void CreateAccelTable();
        void Draw();
    };

}
