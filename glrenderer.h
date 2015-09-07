#pragma once

#define GLM_FORCE_RADIANS

#include "camera.h"
#include "ft_text_renderer.h"
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <glm/glm.hpp>

namespace procdraw {

    struct Vertex {
        // Position
        GLfloat x;
        GLfloat y;
        GLfloat z;
        GLfloat w;
        // Normal
        GLfloat nx;
        GLfloat ny;
        GLfloat nz;
        Vertex(glm::vec3 pos, glm::vec3 normal)
        {
            this->x = pos.x;
            this->y = pos.y;
            this->z = pos.z;
            this->w = 1.0f;
            this->nx = normal.x;
            this->ny = normal.y;
            this->nz = normal.z;
        }
    };


    class GlRenderer {
    public:
        GlRenderer();
        ~GlRenderer();
        void AmbientLightColor(float h, float s, float v);
        void Background(float h, float s, float v);
        void Begin2D();
        void Begin3D();
        void BeginText();
        void Color(float h, float s, float v);
        void Cube();
        void DoSwap();
        int Height();
        void LightColor(float h, float s, float v);
        double MouseX();
        double MouseY();
        void Rect(int x, int y, int w, int h);
        void RotateX(float turns);
        void RotateY(float turns);
        void RotateZ(float turns);
        void Scale(float x, float y, float z);
        void Tetrahedron();
        void Text(int x, int y);
        void Translate(float x, float y, float z);
        int Width();
    private:
        SDL_Window *window_;
        SDL_GLContext glcontext_;
        FtTextRenderer textRenderer_;
        GLuint program_;
        GLuint program2d_;
        GLuint programText_;
        GLuint tetrahedronVertexBuffer_;
        GLuint tetrahedronVao_;
        GLuint cubeIndexBuffer_;
        GLuint cubeVertexBuffer_;
        GLuint cubeVao_;
        GLuint rectangleVertexBuffer_;
        GLuint rectangleVao_;
        GLfloat rectangleVertices_[8] = {};
        GLuint textRectangleVertexBuffer_;
        GLuint textRectangleVao_;
        GLfloat textRectangleVertices_[16] = {};
        GLuint textTexture_;
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
        GLuint CompileShaders2d();
        GLuint CompileShadersText();
        GLuint CompileProgram(const GLchar **vertexShaderSource, const GLchar **fragmentShaderSource);
        void MakeTetrahedronVao();
        void MakeCubeVao();
        void MakeRectangleVao();
        void MakeTextRectangleVao();
        void MakeTextTexture();
        void ResetMatrix();
        void InitLighting();
        void InitMaterial();
        void UpdateUniformsForObject();
    };

}
