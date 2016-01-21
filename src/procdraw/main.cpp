#include "sdl_util.h"
#include "procdraw_app.h"

int main()
{
    procdraw::SDL sdl(SDL_INIT_VIDEO);
    procdraw::ProcdrawApp app;
    return app.MainLoop();
}
