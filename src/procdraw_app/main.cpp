#define SDL_MAIN_HANDLED
#include "procdraw_app/procdraw_app.h"
#include "procdraw_core/graphics/sdl_util.h"

int
main()
{
  procdraw::SDL sdl(SDL_INIT_VIDEO);
  procdraw::ProcdrawApp app;
  return app.MainLoop();
}
