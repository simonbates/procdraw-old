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
        tetrahedronBuffers_ {0, 0}, tetrahedronVao_(0),
        cubeIndexBuffer_(0), cubeBuffers_ {0, 0}, cubeVao_(0)
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
        glDeleteBuffers(2, tetrahedronBuffers_);

        glDeleteVertexArrays(1, &cubeVao_);
        glDeleteBuffers(2, cubeBuffers_);
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
        auto vertex1 = glm::vec3(1.0f, 0.0f, M_SQRT1_2);
        auto vertex2 = glm::vec3(0.0f, 1.0f, -M_SQRT1_2);
        auto vertex3 = glm::vec3(0.0f, -1.0f, -M_SQRT1_2);
        auto vertex4 = glm::vec3(-1.0f, 0.0f, M_SQRT1_2);

        auto face1Normal = TriangleNormal(vertex1, vertex2, vertex4);
        auto face2Normal = TriangleNormal(vertex1, vertex4, vertex3);
        auto face3Normal = TriangleNormal(vertex1, vertex3, vertex2);
        auto face4Normal = TriangleNormal(vertex2, vertex3, vertex4);

        GLfloat positions[] = {
            // Face 1
            vertex1.x, vertex1.y, vertex1.z, 1.0f,
            vertex2.x, vertex2.y, vertex2.z, 1.0f,
            vertex4.x, vertex4.y, vertex4.z, 1.0f,
            // Face 2
            vertex1.x, vertex1.y, vertex1.z, 1.0f,
            vertex4.x, vertex4.y, vertex4.z, 1.0f,
            vertex3.x, vertex3.y, vertex3.z, 1.0f,
            // Face 3
            vertex1.x, vertex1.y, vertex1.z, 1.0f,
            vertex3.x, vertex3.y, vertex3.z, 1.0f,
            vertex2.x, vertex2.y, vertex2.z, 1.0f,
            // Face 4
            vertex2.x, vertex2.y, vertex2.z, 1.0f,
            vertex3.x, vertex3.y, vertex3.z, 1.0f,
            vertex4.x, vertex4.y, vertex4.z, 1.0f
        };

        GLfloat normals[] = {
            face1Normal.x, face1Normal.y, face1Normal.z,
            face1Normal.x, face1Normal.y, face1Normal.z,
            face1Normal.x, face1Normal.y, face1Normal.z,
            face2Normal.x, face2Normal.y, face2Normal.z,
            face2Normal.x, face2Normal.y, face2Normal.z,
            face2Normal.x, face2Normal.y, face2Normal.z,
            face3Normal.x, face3Normal.y, face3Normal.z,
            face3Normal.x, face3Normal.y, face3Normal.z,
            face3Normal.x, face3Normal.y, face3Normal.z,
            face4Normal.x, face4Normal.y, face4Normal.z,
            face4Normal.x, face4Normal.y, face4Normal.z,
            face4Normal.x, face4Normal.y, face4Normal.z
        };

        glGenVertexArrays(1, &tetrahedronVao_);
        glBindVertexArray(tetrahedronVao_);

        glGenBuffers(2, tetrahedronBuffers_);

        // Positions
        glBindBuffer(GL_ARRAY_BUFFER, tetrahedronBuffers_[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(0);

        // Normals
        glBindBuffer(GL_ARRAY_BUFFER, tetrahedronBuffers_[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(1);
    }

    void GlRenderer::MakeCubeVao()
    {
        auto vertex1 = glm::vec3(1.0f, 1.0f, 1.0f);
        auto vertex2 = glm::vec3(1.0f, 1.0f, -1.0f);
        auto vertex3 = glm::vec3(-1.0f, 1.0f, -1.0f);
        auto vertex4 = glm::vec3(-1.0f, 1.0f, 1.0f);
        auto vertex5 = glm::vec3(1.0f, -1.0f, 1.0f);
        auto vertex6 = glm::vec3(1.0f, -1.0f, -1.0f);
        auto vertex7 = glm::vec3(-1.0f, -1.0f, -1.0f);
        auto vertex8 = glm::vec3(-1.0f, -1.0f, 1.0f);

        auto face1Normal = TriangleNormal(vertex1, vertex4, vertex8);
        auto face2Normal = TriangleNormal(vertex1, vertex5, vertex6);
        auto face3Normal = TriangleNormal(vertex2, vertex6, vertex7);
        auto face4Normal = TriangleNormal(vertex3, vertex7, vertex8);
        auto face5Normal = TriangleNormal(vertex1, vertex2, vertex3);
        auto face6Normal = TriangleNormal(vertex5, vertex8, vertex7);

        GLfloat positions[] = {
            // Face 1
            vertex1.x, vertex1.y, vertex1.z, 1.0f,
            vertex4.x, vertex4.y, vertex4.z, 1.0f,
            vertex8.x, vertex8.y, vertex8.z, 1.0f,
            vertex5.x, vertex5.y, vertex5.z, 1.0f,
            // Face 2
            vertex1.x, vertex1.y, vertex1.z, 1.0f,
            vertex5.x, vertex5.y, vertex5.z, 1.0f,
            vertex6.x, vertex6.y, vertex6.z, 1.0f,
            vertex2.x, vertex2.y, vertex2.z, 1.0f,
            // Face 3
            vertex2.x, vertex2.y, vertex2.z, 1.0f,
            vertex6.x, vertex6.y, vertex6.z, 1.0f,
            vertex7.x, vertex7.y, vertex7.z, 1.0f,
            vertex3.x, vertex3.y, vertex3.z, 1.0f,
            // Face 4
            vertex3.x, vertex3.y, vertex3.z, 1.0f,
            vertex7.x, vertex7.y, vertex7.z, 1.0f,
            vertex8.x, vertex8.y, vertex8.z, 1.0f,
            vertex4.x, vertex4.y, vertex4.z, 1.0f,
            // Face 5
            vertex1.x, vertex1.y, vertex1.z, 1.0f,
            vertex2.x, vertex2.y, vertex2.z, 1.0f,
            vertex3.x, vertex3.y, vertex3.z, 1.0f,
            vertex4.x, vertex4.y, vertex4.z, 1.0f,
            // Face 6
            vertex5.x, vertex5.y, vertex5.z, 1.0f,
            vertex8.x, vertex8.y, vertex8.z, 1.0f,
            vertex7.x, vertex7.y, vertex7.z, 1.0f,
            vertex6.x, vertex6.y, vertex6.z, 1.0f
        };

        GLfloat normals[] = {
            // Face 1
            face1Normal.x, face1Normal.y, face1Normal.z,
            face1Normal.x, face1Normal.y, face1Normal.z,
            face1Normal.x, face1Normal.y, face1Normal.z,
            face1Normal.x, face1Normal.y, face1Normal.z,
            // Face 2
            face2Normal.x, face2Normal.y, face2Normal.z,
            face2Normal.x, face2Normal.y, face2Normal.z,
            face2Normal.x, face2Normal.y, face2Normal.z,
            face2Normal.x, face2Normal.y, face2Normal.z,
            // Face 3
            face3Normal.x, face3Normal.y, face3Normal.z,
            face3Normal.x, face3Normal.y, face3Normal.z,
            face3Normal.x, face3Normal.y, face3Normal.z,
            face3Normal.x, face3Normal.y, face3Normal.z,
            // Face 4
            face4Normal.x, face4Normal.y, face4Normal.z,
            face4Normal.x, face4Normal.y, face4Normal.z,
            face4Normal.x, face4Normal.y, face4Normal.z,
            face4Normal.x, face4Normal.y, face4Normal.z,
            // Face 5
            face5Normal.x, face5Normal.y, face5Normal.z,
            face5Normal.x, face5Normal.y, face5Normal.z,
            face5Normal.x, face5Normal.y, face5Normal.z,
            face5Normal.x, face5Normal.y, face5Normal.z,
            // Face 6
            face6Normal.x, face6Normal.y, face6Normal.z,
            face6Normal.x, face6Normal.y, face6Normal.z,
            face6Normal.x, face6Normal.y, face6Normal.z,
            face6Normal.x, face6Normal.y, face6Normal.z
        };

        GLushort indices[] = {
            // Face 1
            0, 1, 2,
            0, 2, 3,
            // Face 2
            4, 5, 6,
            4, 6, 7,
            // Face 3
            8, 9, 10,
            8, 10, 11,
            // Face 4
            12, 13, 14,
            12, 14, 15,
            // Face 5
            16, 17, 18,
            16, 18, 19,
            // Face 6
            20, 21, 22,
            20, 22, 23
        };

        glGenVertexArrays(1, &cubeVao_);
        glBindVertexArray(cubeVao_);

        glGenBuffers(2, cubeBuffers_);

        // Positions
        glBindBuffer(GL_ARRAY_BUFFER, cubeBuffers_[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(0);

        // Normals
        glBindBuffer(GL_ARRAY_BUFFER, cubeBuffers_[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
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
