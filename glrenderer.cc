#include "glrenderer.h"
#include "color.h"
#include "sdl_util.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

namespace procdraw {

    GlRenderer::GlRenderer() :
        window_(nullptr),
        glcontext_(NULL),
        program_(0),
        pointBuffer_(0),
        pointVao_(0),
        triangleBuffer_(0),
        triangleVao_(0)
    {
        CreateWindowAndGlContext();

        program_ = CompileShaders();
        glUseProgram(program_);

        MakePointVao();
        MakeTriangleVao();

        ResetMatrix();
    }

    GlRenderer::~GlRenderer()
    {
        // TODO The constructor/destructor design here is not safe -- if the constructor throws an exception, the destructor will not be called
        glDeleteVertexArrays(1, &pointVao_);
        glDeleteBuffers(1, &pointBuffer_);

        glDeleteVertexArrays(1, &triangleVao_);
        glDeleteBuffers(1, &triangleBuffer_);

        glDeleteProgram(program_);
        if (glcontext_ != NULL) {
            SDL_GL_DeleteContext(glcontext_);
        }
        if (window_ != nullptr) {
            SDL_DestroyWindow(window_);
        }
    }

    double GlRenderer::Width()
    {
        int w, h;
        SDL_GetWindowSize(window_, &w, &h);
        return static_cast<double>(w);
    }

    double GlRenderer::Height()
    {
        int w, h;
        SDL_GetWindowSize(window_, &w, &h);
        return static_cast<double>(h);
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

    void GlRenderer::DoSwap()
    {
        SDL_GL_SwapWindow(window_);
    }

    void GlRenderer::Background(float h, float s, float v)
    {
        float r, g, b;
        Hsv2Rgb(h, s, v, r, g, b);
        glClearColor(r, g, b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ResetMatrix();
    }

    void GlRenderer::Colour(float h, float s, float v)
    {
        float r, g, b;
        Hsv2Rgb(h, s, v, r, g, b);
        // TODO use glGetUniformLocation
        glUniform4f(2, r, g, b, 1.0f);
    }

    void GlRenderer::Point()
    {
        UpdateUniformsForObject();
        glBindVertexArray(pointVao_);
        glPointSize(20.0f);
        glDrawArrays(GL_POINTS, 0, 1);
    }

    void GlRenderer::Triangle()
    {
        UpdateUniformsForObject();
        glBindVertexArray(triangleVao_);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    void GlRenderer::RotateZ(float turns)
    {
        worldMatrix_ = glm::rotate(worldMatrix_,
                                   turns * 2 * static_cast<float>(M_PI),
                                   glm::vec3(0.0f, 0.0f, 1.0f));
    }

    void GlRenderer::CreateWindowAndGlContext()
    {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

        window_ = SDL_CreateWindow("Procdraw", SDL_WINDOWPOS_UNDEFINED,
                                   SDL_WINDOWPOS_UNDEFINED, 640, 480,
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
    }

    GLuint GlRenderer::CompileShaders()
    {
        static const GLchar *vertexShaderSource[] = {
            "#version 430 core                                  \n"
            "layout (location = 0) in vec4 position;            \n"
            "layout (location = 1) uniform mat4 world_matrix;   \n"
            "layout (location = 2) uniform vec4 colour;         \n"
            "out vec4 vs_color;                                 \n"
            "void main(void)                                    \n"
            "{                                                  \n"
            "    gl_Position = world_matrix * position;         \n"
            "    vs_color = colour;                             \n"
            "}                                                  \n"
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

    void GlRenderer::MakePointVao()
    {
        GLfloat position[] = { 0.5f, 0.0f, 0.5f, 1.0f };

        glGenBuffers(1, &pointBuffer_);
        glBindBuffer(GL_ARRAY_BUFFER, pointBuffer_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW);

        glGenVertexArrays(1, &pointVao_);
        glBindVertexArray(pointVao_);

        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(0);
    }

    void GlRenderer::MakeTriangleVao()
    {
        GLfloat positions[] = {
            -0.5f, 0.0f, 0.5f, 1.0f,
            0.5f, 0.0f, 0.5f, 1.0f,
            0.0f, 0.5f, 0.5f, 1.0f
        };

        glGenBuffers(1, &triangleBuffer_);
        glBindBuffer(GL_ARRAY_BUFFER, triangleBuffer_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

        glGenVertexArrays(1, &triangleVao_);
        glBindVertexArray(triangleVao_);

        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(0);
    }

    void GlRenderer::ResetMatrix()
    {
        worldMatrix_ = glm::mat4(1.0f);
    }

    void GlRenderer::UpdateUniformsForObject()
    {
        // TODO use glGetUniformLocation
        glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(worldMatrix_));
    }

}
