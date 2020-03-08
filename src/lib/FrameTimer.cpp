// Copyright 2020 Simon Bates
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "FrameTimer.h"
#include <GLFW/glfw3.h>

namespace Procdraw {

FrameTimer::FrameTimer()
    : timerFrequency_(glfwGetTimerFrequency()),
      lastTimerValue_(glfwGetTimerValue()),
      frameTimes_(100)
{
}

void FrameTimer::RecordFrame()
{
    uint64_t timerValue = glfwGetTimerValue();
    frameTimes_.AddDataPoint(timerValue - lastTimerValue_);
    lastTimerValue_ = timerValue;
}

double FrameTimer::GetFramesPerSecond()
{
    double avg = frameTimes_.GetMean();
    return avg == 0 ? 0 : timerFrequency_ / avg;
}
} // namespace Procdraw
