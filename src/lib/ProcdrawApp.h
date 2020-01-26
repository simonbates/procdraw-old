// Copyright 2020 Simon Bates
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef PROCDRAW_PROCDRAWAPP_H
#define PROCDRAW_PROCDRAWAPP_H

#include "D3D11Graphics.h"
#include <Windows.h>
#include <memory>

namespace Procdraw {

class ProcdrawApp {
public:
    ProcdrawApp(HINSTANCE hInstance, int nCmdShow);
    static LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
    int MainLoop();
    double Width();
    double Height();
    double MouseX();
    double MouseY();

private:
    HINSTANCE hInstance_;
    int nCmdShow_;
    HWND hWnd_;
    std::unique_ptr<D3D11Graphics> graphics_;
    void CreateAppWindow();
    void Draw();
};

} // namespace Procdraw

#endif
