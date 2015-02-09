#pragma once

#include "command_processor.h"
#include "d3d11graphics.h"
#include "workspace.h"
#include "lisp_interpreter.h"
#include "frame_counter.h"
#include <string>
#include <memory>

namespace procdraw {

    class ProcDrawApp : public CommandProcessor {
    public:
        ProcDrawApp(HINSTANCE hInstance, int nCmdShow);
        inline D3D11Graphics* Graphics()
        {
            return graphics_.get();
        }
        static LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
        int MainLoop();
        void DoCommand(const std::string &cmd) override;
        double FramesPerSecond();
        double Width();
        double Height();
        double MouseX();
        double MouseY();
    private:
        HINSTANCE hInstance_;
        int nCmdShow_;
        HWND hWnd_;
        HACCEL hAccel_;
        std::unique_ptr<D3D11Graphics> graphics_;
        std::unique_ptr<Workspace> workspace_;
        std::unique_ptr<Workspace> transcript_;
        FrameCounter frameCounter_;
        LispInterpreter L_;
        void CreateAppWindow();
        void CreateAccelTable();
        void Draw();
    };

}
