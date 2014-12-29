#pragma once

#include "util.h"
#include <Windows.h>

namespace procdraw {

    class FrameCounter {
    public:
        FrameCounter();
        void RecordFrame();
        double GetFramesPerSecond();
    private:
        double perfCountsPerSec_;
        LARGE_INTEGER lastPerfCount_;
        SimpleMovingAverage<decltype(LARGE_INTEGER::QuadPart)> frameTimes_;
    };

}
