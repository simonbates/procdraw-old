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
        void Colour(float h, float s, float v);
        void Point();
        void Triangle();
        int MainLoop();
    private:
        SDL_Window *window_;
        SDL_GLContext glcontext_;
        GLuint program_;
        GLuint pointBuffer_;
        GLuint pointVao_;
        GLuint triangleBuffer_;
        GLuint triangleVao_;
        void CreateWindowAndGlContext();
        GLuint CompileShaders();
        void MakePointVao();
        void MakeTriangleVao();
        void Draw();
    };

}
