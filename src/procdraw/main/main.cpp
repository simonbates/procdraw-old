#define SDL_MAIN_HANDLED

#ifdef PROCDRAW_ENABLE_OPENGL
#include "procdraw/opengl/procdraw_app.h"
#include "procdraw/opengl/sdl_util.h"
#endif

#include "procdraw/core/interpreter/lisp_interpreter.h"
#include <iostream>
#include <string>

int
RunSuperSimpleRepl()
{
  procdraw::LispInterpreter interpreter;
  std::string expr;
  while (true) {
    std::cout << "> ";
    std::getline(std::cin, expr);
    // Note the hardcoded 40 char width
    std::cout << interpreter.PrettyPrintToString(
                   interpreter.Eval(interpreter.Read(expr)), 40)
              << std::endl;
  }
  return 0;
}

int
main()
{
#ifdef PROCDRAW_ENABLE_OPENGL
  procdraw::SDL sdl(SDL_INIT_VIDEO);
  procdraw::ProcdrawApp app;
  return app.MainLoop();
#else
  RunSuperSimpleRepl();
#endif
}
