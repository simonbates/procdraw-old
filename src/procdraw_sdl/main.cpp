#include "sdl_util.h"
#include "procdraw_app_sdl.h"

int main()
{
    procdraw::SDL sdl(SDL_INIT_VIDEO);
    procdraw::ProcDrawAppSdl app;
    return app.Run();
}
