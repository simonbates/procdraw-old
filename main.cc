#include "sdl_util.h"
#include "procdraw_app_sdl.h"
#include "globals.h"

int main()
{
    procdraw::SDL sdl(SDL_INIT_VIDEO);
    procdraw::ProcDrawAppSdl app;
    procdraw::pd_app = &app;
    return app.Run();
}
