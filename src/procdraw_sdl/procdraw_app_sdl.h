#pragma once

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

namespace procdraw {

    class ProcDrawAppSdl {
    public:
        ProcDrawAppSdl();
        ~ProcDrawAppSdl();
        void Background(float h, float s, float v);
        void Point();
        int MainLoop();
    private:
        SDL_Window *window_;
        SDL_GLContext glcontext_;
        GLuint program_;
        GLuint vertexArrayObject_;
        void CreateWindowAndGlContext();
        GLuint CompileShaders();
        void Draw();
    };

}
