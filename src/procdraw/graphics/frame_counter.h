#pragma once

#include "procdraw/simple_moving_average.h"
#include <SDL2/SDL.h>

namespace procdraw {

class FrameCounter {
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
