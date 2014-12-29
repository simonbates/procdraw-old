#include "stdafx.h"
#include "frame_counter.h"

namespace procdraw {

    FrameCounter::FrameCounter() : frameTimes_(100)
    {
        LARGE_INTEGER freq;
        QueryPerformanceFrequency(&freq);
        perfCountsPerSec_ = freq.QuadPart;
        QueryPerformanceCounter(&lastPerfCount_);
    };

    void FrameCounter::RecordFrame()
    {
        LARGE_INTEGER perfCount;
        QueryPerformanceCounter(&perfCount);
        frameTimes_.AddDataPoint(perfCount.QuadPart - lastPerfCount_.QuadPart);
        lastPerfCount_ = perfCount;
    }

    double FrameCounter::GetFramesPerSecond()
    {
        double avg = frameTimes_.GetMean();
        return avg == 0 ? 0 : perfCountsPerSec_ / avg;
    }

}
