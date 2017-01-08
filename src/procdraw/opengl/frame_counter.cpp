#include "procdraw/opengl/frame_counter.h"

namespace procdraw {

FrameCounter::FrameCounter()
    : frameTimes_(100)
{
    perfCountsPerSec_ = SDL_GetPerformanceFrequency();
    lastPerfCount_ = SDL_GetPerformanceCounter();
}

void FrameCounter::RecordFrame()
{
    Uint64 perfCount = SDL_GetPerformanceCounter();
    frameTimes_.AddDataPoint(perfCount - lastPerfCount_);
    lastPerfCount_ = perfCount;
}

double FrameCounter::GetFramesPerSecond()
{
    double avg = frameTimes_.GetMean();
    return avg == 0 ? 0 : perfCountsPerSec_ / avg;
}
}
