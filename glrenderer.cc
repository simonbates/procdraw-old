#include "glrenderer.h"
#include "color.h"
#include "sdl_util.h"
#include "util.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

namespace procdraw {

    GlRenderer::GlRenderer() :
        window_(nullptr),
        glcontext_(NULL),
        program_(0),
        tetrahedronVertexBuffer_(0), tetrahedronVao_(0),
        cubeIndexBuffer_(0), cubeVertexBuffer_(0), cubeVao_(0)
    {
        CreateWindowAndGlContext();

        program_ = CompileShaders();
        glUseProgram(program_);

        MakeTetrahedronVao();
        MakeCubeVao();

        ResetMatrix();
        InitLighting();
        InitMaterial();
    }

    GlRenderer::~GlRenderer()
    {
        // TODO The constructor/destructor design here is not safe -- if the constructor throws an exception, the destructor will not be called

        glDeleteVertexArrays(1, &tetrahedronVao_);
        glDeleteBuffers(1, &tetrahedronVertexBuffer_);

        glDeleteVertexArrays(1, &cubeVao_);
        glDeleteBuffers(1, &cubeVertexBuffer_);
        glDeleteBuffers(1, &cubeIndexBuffer_);

        glDeleteProgram(program_);

        if (glcontext_ != NULL) {
            SDL_GL_DeleteContext(glcontext_);
        }

        if (window_ != nullptr) {
            SDL_DestroyWindow(window_);
        }
    }

    void GlRenderer::AmbientLightColor(float h, float s, float v)
    {
        float r, g, b;
        Hsv2Rgb(h, s, v, r, g, b);
        ambientLightColor_.x = r;
        ambientLightColor_.y = g;
        ambientLightColor_.z = b;
        ambientLightColor_.w = 0.0f;
    }

    void GlRenderer::Background(float h, float s, float v)
    {
        float r, g, b;
        Hsv2Rgb(h, s, v, r, g, b);
        glClearColor(r, g, b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ResetMatrix();
    }

    void GlRenderer::Color(float h, float s, float v)
    {
        Hsv2Rgb(h, s, v, materialR_, materialG_, materialB_);
    }

    void GlRenderer::Cube()
    {
        UpdateUniformsForObject();
        glBindVertexArray(cubeVao_);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
    }

    void GlRenderer::DoSwap()
    {
        SDL_GL_SwapWindow(window_);
    }

    double GlRenderer::Height()
    {
        int w, h;
        SDL_GetWindowSize(window_, &w, &h);
        return static_cast<double>(h);
    }

    void GlRenderer::LightColor(float h, float s, float v)
    {
        float r, g, b;
        Hsv2Rgb(h, s, v, r, g, b);
        lightColor_.x = r;
        lightColor_.y = g;
        lightColor_.z = b;
        lightColor_.w = 0.0f;
    }

    double GlRenderer::MouseX()
    {
        int x, y;
        SDL_GetMouseState(&x, &y);
        return static_cast<double>(x) / (Width() - 1);
    }

    double GlRenderer::MouseY()
    {
        int x, y;
        SDL_GetMouseState(&x, &y);
        return static_cast<double>(y) / (Height() - 1);
    }

    void GlRenderer::RotateX(float turns)
    {
        worldMatrix_ = glm::rotate(worldMatrix_,
                                   turns * 2 * static_cast<float>(M_PI),
                                   glm::vec3(1.0f, 0.0f, 0.0f));
    }

    void GlRenderer::RotateY(float turns)
    {
        worldMatrix_ = glm::rotate(worldMatrix_,
                                   turns * 2 * static_cast<float>(M_PI),
                                   glm::vec3(0.0f, 1.0f, 0.0f));
    }

    void GlRenderer::RotateZ(float turns)
    {
        worldMatrix_ = glm::rotate(worldMatrix_,
                                   turns * 2 * static_cast<float>(M_PI),
                                   glm::vec3(0.0f, 0.0f, 1.0f));
    }

    void GlRenderer::Scale(float x, float y, float z)
    {
        worldMatrix_ = glm::scale(worldMatrix_, glm::vec3(x, y, z));
    }

    void GlRenderer::Tetrahedron()
    {
        UpdateUniformsForObject();
        glBindVertexArray(tetrahedronVao_);
        glDrawArrays(GL_TRIANGLES, 0, 12);
    }

    void GlRenderer::Translate(float x, float y, float z)
    {
        worldMatrix_ = glm::translate(worldMatrix_, glm::vec3(x, y, z));
    }

    double GlRenderer::Width()
    {
        int w, h;
        SDL_GetWindowSize(window_, &w, &h);
        return static_cast<double>(w);
    }

    void GlRenderer::CreateWindowAndGlContext()
    {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

        window_ = SDL_CreateWindow("Procdraw", SDL_WINDOWPOS_UNDEFINED,
                                   SDL_WINDOWPOS_UNDEFINED, 640, 640,
                                   SDL_WINDOW_OPENGL);
        if (window_ == nullptr) {
            ThrowSdlError();
        }

        glcontext_ = SDL_GL_CreateContext(window_);
        if (glcontext_ == NULL) {
            ThrowSdlError();
        }

        glewExperimental = GL_TRUE;
        ThrowOnGlewError(glewInit());

        std::cout << "GLEW: " << glewGetString(GLEW_VERSION) << std::endl;
        std::cout << "OpenGL vendor: " << glGetString(GL_VENDOR) << std::endl;
        std::cout << "OpenGL renderer: " << glGetString(GL_RENDERER) << std::endl;
        std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
        std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
    }

    GLuint GlRenderer::CompileShaders()
    {
        static const GLchar *vertexShaderSource[] = {
            "#version 430 core                                                          \n"
            "layout (location = 0) in vec4 position;                                    \n"
            "layout (location = 1) in vec3 normal;                                      \n"
            "layout (location = 2) uniform mat4 world_view_projection;                  \n"
            "layout (location = 3) uniform vec4 light_direction;                        \n"
            "layout (location = 4) uniform vec4 light_color;                            \n"
            "layout (location = 5) uniform vec4 ambient_light_color;                    \n"
            "layout (location = 6) uniform vec4 material_color;                         \n"
            "out vec4 vs_color;                                                         \n"
            "void main(void)                                                            \n"
            "{                                                                          \n"
            "    gl_Position = world_view_projection * position;                        \n"
            "    float diff_factor = max(dot(normal, light_direction.xyz), 0);          \n"
            "    vec4 diff_component = (light_color * material_color * diff_factor);    \n"
            "    vec4 ambient_component = (ambient_light_color * material_color);       \n"
            "    vs_color.rgb = diff_component.rgb + ambient_component.rgb;             \n"
            "    vs_color.a = material_color.a;                                         \n"
            "}                                                                          \n"
        };

        static const GLchar *fragmentShaderSource[] = {
            "#version 430 core                                  \n"
            "in vec4 vs_color;                                  \n"
            "out vec4 color;                                    \n"
            "void main(void)                                    \n"
            "{                                                  \n"
            "    color = vs_color;                              \n"
            "}                                                  \n"
        };

        // Vertex shader
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, vertexShaderSource, NULL);
        glCompileShader(vertexShader);

        // Fragment shader
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);

        // Program
        GLuint program = glCreateProgram();
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return program;
    }

    void GlRenderer::MakeTetrahedronVao()
    {
        auto p1 = glm::vec3(1.0f, 0.0f, M_SQRT1_2);
        auto p2 = glm::vec3(0.0f, 1.0f, -M_SQRT1_2);
        auto p3 = glm::vec3(0.0f, -1.0f, -M_SQRT1_2);
        auto p4 = glm::vec3(-1.0f, 0.0f, M_SQRT1_2);

        auto n1 = TriangleNormal(p1, p2, p4);
        auto n2 = TriangleNormal(p1, p4, p3);
        auto n3 = TriangleNormal(p1, p3, p2);
        auto n4 = TriangleNormal(p2, p3, p4);

        Vertex vertices[] = {
            Vertex(p1, n1), Vertex(p2, n1), Vertex(p4, n1),
            Vertex(p1, n2), Vertex(p4, n2), Vertex(p3, n2),
            Vertex(p1, n3), Vertex(p3, n3), Vertex(p2, n3),
            Vertex(p2, n4), Vertex(p3, n4), Vertex(p4, n4)
        };

        glGenVertexArrays(1, &tetrahedronVao_);
        glBindVertexArray(tetrahedronVao_);

        glGenBuffers(1, &tetrahedronVertexBuffer_);
        glBindBuffer(GL_ARRAY_BUFFER, tetrahedronVertexBuffer_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Positions
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, x));
        glEnableVertexAttribArray(0);

        // Normals
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, nx));
        glEnableVertexAttribArray(1);
    }

    void GlRenderer::MakeCubeVao()
    {
        auto p1 = glm::vec3(1.0f, 1.0f, 1.0f);
        auto p2 = glm::vec3(1.0f, 1.0f, -1.0f);
        auto p3 = glm::vec3(-1.0f, 1.0f, -1.0f);
        auto p4 = glm::vec3(-1.0f, 1.0f, 1.0f);
        auto p5 = glm::vec3(1.0f, -1.0f, 1.0f);
        auto p6 = glm::vec3(1.0f, -1.0f, -1.0f);
        auto p7 = glm::vec3(-1.0f, -1.0f, -1.0f);
        auto p8 = glm::vec3(-1.0f, -1.0f, 1.0f);

        auto n1 = TriangleNormal(p1, p4, p8);
        auto n2 = TriangleNormal(p1, p5, p6);
        auto n3 = TriangleNormal(p2, p6, p7);
        auto n4 = TriangleNormal(p3, p7, p8);
        auto n5 = TriangleNormal(p1, p2, p3);
        auto n6 = TriangleNormal(p5, p8, p7);

        Vertex vertices[] = {
            Vertex(p1, n1), Vertex(p4, n1), Vertex(p8, n1), Vertex(p5, n1),
            Vertex(p1, n2), Vertex(p5, n2), Vertex(p6, n2), Vertex(p2, n2),
            Vertex(p2, n3), Vertex(p6, n3), Vertex(p7, n3), Vertex(p3, n3),
            Vertex(p3, n4), Vertex(p7, n4), Vertex(p8, n4), Vertex(p4, n4),
            Vertex(p1, n5), Vertex(p2, n5), Vertex(p3, n5), Vertex(p4, n5),
            Vertex(p5, n6), Vertex(p8, n6), Vertex(p7, n6), Vertex(p6, n6)
        };

        GLushort indices[] = {
            0, 1, 2,
            0, 2, 3,
            4, 5, 6,
            4, 6, 7,
            8, 9, 10,
            8, 10, 11,
            12, 13, 14,
            12, 14, 15,
            16, 17, 18,
            16, 18, 19,
            20, 21, 22,
            20, 22, 23
        };

        glGenVertexArrays(1, &cubeVao_);
        glBindVertexArray(cubeVao_);

        // Bind vertex buffer
        glGenBuffers(1, &cubeVertexBuffer_);
        glBindBuffer(GL_ARRAY_BUFFER, cubeVertexBuffer_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Positions
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, x));
        glEnableVertexAttribArray(0);

        // Normals
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, nx));
        glEnableVertexAttribArray(1);

        // Indices
        glGenBuffers(1, &cubeIndexBuffer_);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeIndexBuffer_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    }

    void GlRenderer::ResetMatrix()
    {
        worldMatrix_ = glm::mat4(1.0f);
    }

    void GlRenderer::InitLighting()
    {
        lightDirection_ = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
        lightColor_ = glm::vec4(0.5f, 0.5f, 0.5f, 0.0f);
        ambientLightColor_ = glm::vec4(0.5f, 0.5f, 0.5f, 0.0f);
    }

    void GlRenderer::InitMaterial()
    {
        materialR_ = 0.8f;
        materialG_ = 0.8f;
        materialB_ = 0.8f;
    }

    void GlRenderer::UpdateUniformsForObject()
    {
        auto worldViewProjection = camera_.ViewProjectionMatrix() * worldMatrix_;
        auto inverseWorldMatrix = glm::inverse(worldMatrix_);
        auto modelSpaceLightDirection = glm::normalize(inverseWorldMatrix * lightDirection_);

        // TODO use glGetUniformLocation
        glUniformMatrix4fv(2, 1, GL_FALSE, glm::value_ptr(worldViewProjection));
        glUniform4fv(3, 1, glm::value_ptr(modelSpaceLightDirection));
        glUniform4fv(4, 1, glm::value_ptr(lightColor_));
        glUniform4fv(5, 1, glm::value_ptr(ambientLightColor_));
        glUniform4f(6, materialR_, materialG_, materialB_, 1.0f);
    }

}
