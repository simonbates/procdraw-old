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

#include <glad/glad.h>
#include "ProcdrawApp.h"
#include "../lib/ProcdrawMath.h"
#include <GLFW/glfw3.h>
#include <memory>
#include <plog/Log.h>

namespace Procdraw {

ProcdrawAppWindow::ProcdrawAppWindow()
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

    window_ = glfwCreateWindow(640, 640, "Procdraw OpenGL", nullptr, nullptr);

    if (!window_) {
        throw std::exception("glfwCreateWindow() failed");
    }
}

ProcdrawAppWindow::~ProcdrawAppWindow()
{
    glfwDestroyWindow(window_);
}

ProcdrawApp::ProcdrawApp()
{
    glfwMakeContextCurrent(window_.Window());
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    PLOG_INFO << "OpenGL vendor: " << glGetString(GL_VENDOR);
    PLOG_INFO << "OpenGL renderer: " << glGetString(GL_RENDERER);
    PLOG_INFO << "OpenGL version: " << glGetString(GL_VERSION);
    PLOG_INFO << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION);

    renderer_ = std::make_unique<GLRenderer>();
}

int ProcdrawApp::MainLoop()
{
    while (!glfwWindowShouldClose(window_.Window())) {
        Draw();
        glfwSwapBuffers(window_.Window());
        frameTimer_.RecordFrame();
        glfwPollEvents();
    }
    return 0;
}

double ProcdrawApp::Width()
{
    int width, height;
    glfwGetWindowSize(window_.Window(), &width, &height);
    return static_cast<double>(width);
}

double ProcdrawApp::Height()
{
    int width, height;
    glfwGetWindowSize(window_.Window(), &width, &height);
    return static_cast<double>(height);
}

double ProcdrawApp::MouseX()
{
    double xpos, ypos;
    glfwGetCursorPos(window_.Window(), &xpos, &ypos);
    return Procdraw::Clamp(xpos / (Width() - 1), 0.0, 1.0);
}

double ProcdrawApp::MouseY()
{
    double xpos, ypos;
    glfwGetCursorPos(window_.Window(), &xpos, &ypos);
    return Procdraw::Clamp(ypos / (Height() - 1), 0.0, 1.0);
}

void ProcdrawApp::Draw()
{
    renderer_->Background(200, 0.6f, 0.9f);
    renderer_->RotateX(Procdraw::Lerp(-1.0, 1.0, MouseY()));
    renderer_->RotateY(Procdraw::Lerp(-1.0, 1.0, MouseX()));
    renderer_->Colour(7, 0.7f, 0.7f);
    renderer_->DrawCube();
}

} // namespace Procdraw
