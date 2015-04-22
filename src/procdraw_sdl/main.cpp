#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

int main()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window *win = SDL_CreateWindow("ProcDraw", SDL_WINDOWPOS_UNDEFINED,
                                       SDL_WINDOWPOS_UNDEFINED, 640, 480,
                                       SDL_WINDOW_OPENGL);
    if (win == nullptr) {
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_GLContext glcontext = SDL_GL_CreateContext(win);
    if (glcontext == nullptr) {
        std::cout << "SDL_GL_CreateContext Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    glClearColor(0, 0, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    SDL_GL_SwapWindow(win);

    SDL_Event e;
    bool quit = false;

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }
        SDL_Delay(20);
    }

    SDL_GL_DeleteContext(glcontext);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}
