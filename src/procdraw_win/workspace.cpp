#include "stdafx.h"
#include "workspace.h"
#include "win_util.h"
#include "resource.h"
#include <stdexcept>

#define ID_EDIT 1

namespace procdraw {

    Workspace::Workspace(HINSTANCE hInstance, int nCmdShow,
            const std::wstring &name, bool readonly, CommandProcessor *cmdProcessor) :
        hInstance_(hInstance),
        hwnd_(nullptr),
        hwndEdit_(nullptr),
        readonly_(readonly),
        cmdProcessor_(cmdProcessor)
    {
        CreateWorkspaceWindow(name);
        ShowWindow(hwnd_, nCmdShow);
        UpdateWindow(hwnd_);
    }

    void Workspace::RegisterWindowClass(HINSTANCE hInstance)
    {
        WNDCLASSEX wcex;
        wcex.cbSize = sizeof(WNDCLASSEX);
        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = WindowProc;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = 0;
        wcex.hInstance = hInstance;
        wcex.hIcon = nullptr;
        wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wcex.lpszMenuName = nullptr;
        wcex.lpszClassName = L"ProcDrawWorkspace";
        wcex.hIconSm = nullptr;

        if (!RegisterClassEx(&wcex)) {
            throw std::runtime_error("Failed to register window class");
        }
    }

    void Workspace::CreateWorkspaceWindow(const std::wstring &name)
    {
        RECT rc = { 0, 0, 800, 600 };
        AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
        hwnd_ = CreateWindow(L"ProcDrawWorkspace", name.c_str(), WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left,
            rc.bottom - rc.top, nullptr, nullptr, hInstance_,
            this);

        if (!hwnd_) {
            throw std::runtime_error("Failed to create window");
        }

        SetWindowLongPtr(hwnd_, GWLP_USERDATA, (LONG_PTR)this);
    }

    LRESULT CALLBACK Workspace::WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        LONG_PTR ptr = GetWindowLongPtr(hwnd, GWLP_USERDATA);
        Workspace *workspace = reinterpret_cast<Workspace*>(ptr);

        switch (message) {
        case WM_CREATE:
        {
            CREATESTRUCT *pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
            workspace = reinterpret_cast<Workspace*>(pCreate->lpCreateParams);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)workspace);

            DWORD style = WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL |
                ES_LEFT | ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL;
            if (workspace->readonly_) {
                style |= ES_READONLY;
            }

            workspace->hwndEdit_ = CreateWindow(L"EDIT", NULL, style,
                0, 0, 0, 0, hwnd, (HMENU)ID_EDIT,
                pCreate->hInstance, NULL);

            HDC hdc = GetDC(hwnd);
            int fontHeight = -MulDiv(11, GetDeviceCaps(hdc, LOGPIXELSY), 72);

            HFONT hFont = CreateFont(fontHeight, 0, 0, 0, FW_REGULAR, FALSE, FALSE,
                FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS,
                CLEARTYPE_QUALITY, FIXED_PITCH, TEXT("Consolas"));

            SendMessage(workspace->hwndEdit_, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
            return 0;
        }
        case WM_SETFOCUS:
            SetFocus(workspace->hwndEdit_);
            return 0;
        case WM_SIZE:
            MoveWindow(workspace->hwndEdit_, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);
            return 0;
        case WM_COMMAND:
            if (LOWORD(wParam) == IDM_DO_COMMAND)
            {
                // Get the current selection and send it to DoCommand
                DWORD selStart;
                DWORD selEnd;
                SendMessage(workspace->hwndEdit_, EM_GETSEL, (WPARAM)&selStart, (LPARAM)&selEnd);
                int selLength = selEnd - selStart;
                if (selLength > 0) {
                    auto numWchars = SendMessage(workspace->hwndEdit_, WM_GETTEXTLENGTH, 0, 0);
                    numWchars = numWchars + 1; // add 1 for null terminator
                    std::vector<WCHAR> text(numWchars);
                    SendMessage(workspace->hwndEdit_, WM_GETTEXT, numWchars, (LPARAM)text.data());
                    workspace->DoCommand(Utf16ToUtf8(&text[selStart], selLength));
                }
                return 0;
            }
        }

        return DefWindowProc(hwnd, message, wParam, lParam);
    }

    HWND Workspace::GetHwnd()
    {
        return hwnd_;
    }

    void Workspace::AddLine(const std::string &text)
    {
        auto wtext = Utf8ToUtf16(text + "\r\n");

        int ndx = GetWindowTextLength(hwndEdit_);
        SendMessage(hwndEdit_, EM_SETSEL, (WPARAM)ndx, (LPARAM)ndx);
        SendMessage(hwndEdit_, EM_REPLACESEL, 0, (LPARAM)wtext.data());
    }

    void Workspace::DoCommand(const std::string &cmd)
    {
        cmdProcessor_->DoCommand(cmd);
    }

}
