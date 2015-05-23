#pragma once

#define GLM_FORCE_RADIANS

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <glm/glm.hpp>

namespace procdraw {

    class ProcDrawAppSdl {
    public:
        ProcDrawAppSdl();
        ~ProcDrawAppSdl();
        double Width();
        double Height();
        double MouseX();
        double MouseY();
        void Background(float h, float s, float v);
        void Colour(float h, float s, float v);
        void Point();
        void Triangle();
        void RotateZ(float turns);
        int MainLoop();
    private:
        SDL_Window *window_;
        SDL_GLContext glcontext_;
        GLuint program_;
        GLuint pointBuffer_;
        GLuint pointVao_;
        GLuint triangleBuffer_;
        GLuint triangleVao_;
        glm::mat4 worldMatrix_;
        void CreateWindowAndGlContext();
        GLuint CompileShaders();
        void MakePointVao();
        void MakeTriangleVao();
        void ResetMatrix();
        void UpdateUniformsForObject();
        void Draw();
    };

}
