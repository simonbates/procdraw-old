#pragma once

#define GLM_FORCE_RADIANS

#include "camera.h"
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <glm/glm.hpp>

namespace procdraw {

    class GlRenderer {
    public:
        GlRenderer();
        ~GlRenderer();
        void AmbientLightColor(float h, float s, float v);
        void Background(float h, float s, float v);
        void Color(float h, float s, float v);
        void DoSwap();
        double Height();
        void LightColor(float h, float s, float v);
        double MouseX();
        double MouseY();
        void RotateX(float turns);
        void RotateY(float turns);
        void RotateZ(float turns);
        void Scale(float x, float y, float z);
        void Tetrahedron();
        void Translate(float x, float y, float z);
        double Width();
    private:
        SDL_Window *window_;
        SDL_GLContext glcontext_;
        GLuint program_;
        GLuint tetrahedronBuffers_[2];
        GLuint tetrahedronVao_;
        Camera camera_;
        glm::mat4 worldMatrix_;
        glm::vec4 lightDirection_;
        glm::vec4 lightColor_;
        glm::vec4 ambientLightColor_;
        float materialR_;
        float materialG_;
        float materialB_;
        void CreateWindowAndGlContext();
        GLuint CompileShaders();
        void MakeTetrahedronVao();
        void ResetMatrix();
        void InitLighting();
        void InitMaterial();
        void UpdateUniformsForObject();
    };

}
