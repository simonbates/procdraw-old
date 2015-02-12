#include "stdafx.h"
#include "procdraw_app.h"
#include "procdraw_app_lisp.h"
#include "util.h"
#include "win_util.h"
#include "signals.h"

namespace procdraw {

    ProcDrawApp::ProcDrawApp(HINSTANCE hInstance, int nCmdShow) :
        hInstance_(hInstance),
        nCmdShow_(nCmdShow),
        hWnd_(nullptr),
        hAccel_(nullptr)
    {
        CreateAppWindow();

        hAccel_ = LoadAccelerators(hInstance, L"ProcDraw");
        if (!hAccel_) {
            throw std::runtime_error("Failed to load accelerators");
        }

        graphics_ = std::unique_ptr<D3D11Graphics>(new D3D11Graphics(hWnd_));
        Workspace::RegisterWindowClass(hInstance);
        workspace_ = std::unique_ptr<Workspace>(new Workspace(hInstance, nCmdShow, L"Workspace", false, this));
        transcript_ = std::unique_ptr<Workspace>(new Workspace(hInstance, nCmdShow, L"Transcript", true, this));

        RegisterSignals(&L_);
        RegisterProcDrawAppFunctionsForLisp(this, &L_);

        workspace_->AddLine("To evaluate a Lisp expression, select it and press Ctrl+Return");
        workspace_->AddLine("");
    }

    int ProcDrawApp::MainLoop()
    {
        MSG msg = { 0 };
        while (WM_QUIT != msg.message) {
            if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
                // TODO Is this the best? Asking workspace for its window?
                // TODO Alternatively we could ask workspace to translate the accelerator
                if (!IsChild(workspace_->GetHwnd(), msg.hwnd) || !TranslateAccelerator(workspace_->GetHwnd(), hAccel_, &msg)) {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }
            }
            else {
                Draw();
                graphics_->Present();
                frameCounter_.RecordFrame();
            }
        }
        return (int)msg.wParam;
    }

    void ProcDrawApp::DoCommand(const std::string &cmd)
    {
        std::string val;
        try {
            val = L_.PrintString(L_.Eval(L_.Read(cmd)));
        }
        catch (std::exception e) {
            val = e.what();
        }
        transcript_->AddLine(val);
    }

    double ProcDrawApp::FramesPerSecond()
    {
        return frameCounter_.GetFramesPerSecond();
    }

    double ProcDrawApp::Width()
    {
        int width = 0;
        RECT rect;
        if (GetClientRect(hWnd_, &rect)) {
            width = rect.right - rect.left;
        }
        return static_cast<double>(width);
    }

    double ProcDrawApp::Height()
    {
        int height = 0;
        RECT rect;
        if (GetClientRect(hWnd_, &rect)) {
            height = rect.bottom - rect.top;
        }
        return static_cast<double>(height);
    }

    double ProcDrawApp::MouseX()
    {
        POINT pt;
        GetCursorPos(&pt);
        ScreenToClient(hWnd_, &pt);
        return Clamp(static_cast<double>(pt.x) / (Width() - 1), 0.0, 1.0);
    }

    double ProcDrawApp::MouseY()
    {
        POINT pt;
        GetCursorPos(&pt);
        ScreenToClient(hWnd_, &pt);
        return Clamp(static_cast<double>(pt.y) / (Height() - 1), 0.0, 1.0);
    }

    void ProcDrawApp::CreateAppWindow()
    {
        // Register class
        WNDCLASSEX wcex;
        wcex.cbSize = sizeof(WNDCLASSEX);
        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = WindowProc;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = 0;
        wcex.hInstance = hInstance_;
        wcex.hIcon = nullptr;
        wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wcex.lpszMenuName = nullptr;
        wcex.lpszClassName = L"ProcDrawWindowClass";
        wcex.hIconSm = nullptr;
        if (!RegisterClassEx(&wcex)) {
            throw std::runtime_error("Failed to register window class");
        }

        // Create window
        RECT rc = { 0, 0, 640, 640 };
        AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
        hWnd_ = CreateWindow(L"ProcDrawWindowClass", L"ProcDraw", WS_OVERLAPPEDWINDOW,
                             CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left,
                             rc.bottom - rc.top, nullptr, nullptr, hInstance_,
                             nullptr);

        if (!hWnd_) {
            throw std::runtime_error("Failed to create window");
        }

        SetWindowLongPtr(hWnd_, GWLP_USERDATA, (LONG_PTR)this);

        ShowWindow(hWnd_, nCmdShow_);
    }

    LRESULT CALLBACK ProcDrawApp::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        HDC hdc;
        PAINTSTRUCT ps;

        LONG_PTR ptr = GetWindowLongPtr(hWnd, GWLP_USERDATA);
        ProcDrawApp *app = reinterpret_cast<ProcDrawApp*>(ptr);

        switch (message) {
        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);
            EndPaint(hWnd, &ps);
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        }

        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    void ProcDrawApp::Draw()
    {
        L_.Apply("clear-stepped-signals");
        L_.Apply("draw");
    }

}
