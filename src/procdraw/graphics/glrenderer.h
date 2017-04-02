#pragma once

#define GLM_FORCE_RADIANS

#include "procdraw/graphics/bitmap_text_renderer.h"
#include "procdraw/graphics/camera.h"
#include <GL/gl3w.h>
#include PROCDRAW_SDL2_H
#include PROCDRAW_SDL2_OPENGL_H
#include "glm/glm.hpp"
#include <memory>
#include <string>

namespace procdraw {

class GlRenderer {
public:
    GlRenderer();
    ~GlRenderer();
    void AmbientLightColor(float h, float s, float v);
    void Background(float h, float s, float v);
    void Begin2D();
    void Begin3D();
    void BeginInverse();
    void BeginText();
    void Color(float h, float s, float v, float a = 1.0f);
    void Cube();
    void DoSwap();
    void DrawBlockCursorBackground(int cursorX,
                                   int cursorY,
                                   int cursorWidth,
                                   int cursorHeight);
    void DrawBlockCursorInversion(int cursorX,
                                  int cursorY,
                                  int cursorWidth,
                                  int cursorHeight);
    void DrawText(int x,
                  int y,
                  const TextLayout<GLfloat>& layout,
                  TextLayout<GLfloat>::size_type startLineNum,
                  TextLayout<GLfloat>::size_type endLineNum);
    void EndInverse();
    int GetLinespace();
    int Height();
    TextLayout<GLfloat> LayOutText(const std::string& text,
                                   int maxLineWidthPixels);
    void LightColor(float h, float s, float v);
    double MouseX();
    double MouseY();
    void Rect(int x, int y, int w, int h);
    void RotateX(float turns);
    void RotateY(float turns);
    void RotateZ(float turns);
    void Scale(float factor);
    void Tetrahedron();
    void Translate(float x, float y, float z);
    int Width();

private:
    SDL_Window* window_;
    SDL_GLContext glcontext_;
    std::unique_ptr<BitmapTextRenderer> textRenderer_;
    GLuint program_;
    GLint worldViewProjectionLoc_;
    GLint lightDirectionLoc_;
    GLint lightColorLoc_;
    GLint ambientLightColorLoc_;
    GLint materialColorLoc_;
    GLuint program2d_;
    GLint projection2dLoc_;
    GLint materialColor2dLoc_;
    GLuint tetrahedronVertexBuffer_;
    GLuint tetrahedronVao_;
    GLuint cubeIndexBuffer_;
    GLuint cubeVertexBuffer_;
    GLuint cubeVao_;
    GLuint rectangleVertexBuffer_;
    GLuint rectangleVao_;
    GLfloat rectangleVertices_[8] = {};
    Camera camera_;
    glm::mat4 worldMatrix_;
    glm::vec4 lightDirection_;
    glm::vec4 lightColor_;
    glm::vec4 ambientLightColor_;
    float materialR_;
    float materialG_;
    float materialB_;
    float materialA_;
    void CreateWindowAndGlContext();
    void SetDefaultBlend();
    void CompileShaders();
    void CompileShaders2d();
    void MakeTetrahedronVao();
    void MakeCubeVao();
    void MakeRectangleVao();
    void ResetMatrix();
    void InitLighting();
    void InitMaterial();
    void UpdateUniformsForObject();
};
}
