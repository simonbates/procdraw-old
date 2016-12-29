#pragma once

#include "procdraw_core/text_layout.h"
#include <GL/gl3w.h>
#include <string>

namespace procdraw {

struct ConsoleLine
{
  std::string text;
  TextLayout<GLfloat> layout;
  explicit ConsoleLine(const std::string& text)
    : text(text)
  {
  }
};
}
