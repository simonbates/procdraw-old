#pragma once

#include "procdraw_core/simple_moving_average.h"
#include PROCDRAW_SDL2_H

namespace procdraw {

class FrameCounter
{
public:
  FrameCounter();
  void RecordFrame();
  double GetFramesPerSecond();

private:
  double perfCountsPerSec_;
  Uint64 lastPerfCount_;
  SimpleMovingAverage<Uint64> frameTimes_;
};
}
