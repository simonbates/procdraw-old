#define SDL_MAIN_HANDLED
#include "procdraw/graphics/sdl_util.h"
#include "procdraw/procdraw/procdraw_app.h"

int main()
{
    procdraw::SDL sdl(SDL_INIT_VIDEO);
    procdraw::ProcdrawApp app;
    return app.MainLoop();
}
