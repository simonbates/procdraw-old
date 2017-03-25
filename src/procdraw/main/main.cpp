#define SDL_MAIN_HANDLED

#ifdef _WIN32
#include <windows.h>
#endif

#include "procdraw/graphics/sdl_util.h"
#include "procdraw/procdraw_app.h"

int main()
{
    procdraw::SDL sdl(SDL_INIT_VIDEO);
    procdraw::ProcdrawApp app;
    return app.MainLoop();
}

#ifdef _WIN32
int CALLBACK WinMain(
    _In_ HINSTANCE hInstance,
    _In_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nCmdShow)
{
    // Delegate to the main() function
    return main();
}
#endif
