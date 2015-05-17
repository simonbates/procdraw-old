#include "procdraw_app_sdl.h"
#include "color.h"
#include "sdl_util.h"
#include <iostream>
#include <stdexcept>

namespace procdraw {

    ProcDrawAppSdl::ProcDrawAppSdl() :
        window_(nullptr),
        glcontext_(NULL),
        program_(0),
        vertexArrayObjectAllocated_(false)
    {
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            ThrowSdlError();
        }

        CreateWindowAndGlContext();

        program_ = CompileShaders();
        glUseProgram(program_);

        // Vertex array object
        glGenVertexArrays(1, &vertexArrayObject_);
        vertexArrayObjectAllocated_ = true;
        glBindVertexArray(vertexArrayObject_);
    }

    ProcDrawAppSdl::~ProcDrawAppSdl()
    {
        if (vertexArrayObjectAllocated_) {
            glDeleteVertexArrays(1, &vertexArrayObject_);
        }
        if (program_ != 0) {
            glDeleteProgram(program_);
        }
        if (glcontext_ != NULL) {
            SDL_GL_DeleteContext(glcontext_);
        }
        if (window_ != nullptr) {
            SDL_DestroyWindow(window_);
        }
        SDL_Quit();
    }

    void ProcDrawAppSdl::Background(float h, float s, float v)
    {
        float r, g, b;
        Hsv2Rgb(h, s, v, r, g, b);
        glClearColor(r, g, b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void ProcDrawAppSdl::Point()
    {
        glPointSize(20.0f);
        glDrawArrays(GL_POINTS, 0, 1);
    }

    int ProcDrawAppSdl::MainLoop()
    {
        SDL_Event e;
        bool quit = false;

        while (!quit) {
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                }
            }
            Draw();
            SDL_GL_SwapWindow(window_);
            SDL_Delay(50);
        }

        return 0;
    }

    void ProcDrawAppSdl::CreateWindowAndGlContext()
    {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

        window_ = SDL_CreateWindow("ProcDraw", SDL_WINDOWPOS_UNDEFINED,
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

    GLuint ProcDrawAppSdl::CompileShaders()
    {
        static const GLchar *vertexShaderSource[] = {
            "#version 430 core                              \n"
            "void main(void)                                \n"
            "{                                              \n"
            "    gl_Position = vec4(0.0, 0.0, 0.5, 1.0);    \n"
            "}                                              \n"
        };

        static const GLchar *fragmentShaderSource[] = {
            "#version 430 core                              \n"
            "out vec4 color;                                \n"
            "void main(void)                                \n"
            "{                                              \n"
            "    color = vec4(1.0, 0.0, 0.0, 1.0);          \n"
            "}                                              \n"
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

    void ProcDrawAppSdl::Draw()
    {
        Background(200.0f, 0.6f, 0.9f);
        Point();
    }

}
