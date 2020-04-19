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

#pragma once

#include "FrameTimer.h"
#include "glrenderer/GLRenderer.h"
#include <GLFW/glfw3.h>
#include <memory>
#include <stdexcept>

namespace Procdraw {

class ProcdrawAppWindow {
public:
    ProcdrawAppWindow();
    ~ProcdrawAppWindow();
    GLFWwindow* Window()
    {
        return window_;
    }

private:
    GLFWwindow* window_;
};

class ProcdrawApp {
public:
    ProcdrawApp();
    int MainLoop();
    double Width();
    double Height();
    double MouseX();
    double MouseY();

private:
    ProcdrawAppWindow window_;
    std::unique_ptr<GLRenderer> renderer_;
    FrameTimer frameTimer_;
    void Draw();
};

} // namespace Procdraw
