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

#ifndef PROCDRAW_SIMPLEMOVINGAVERAGE_H
#define PROCDRAW_SIMPLEMOVINGAVERAGE_H

#include <stdexcept>
#include <vector>

namespace Procdraw {

template <typename T>
class SimpleMovingAverage {
public:
    explicit SimpleMovingAverage(int numDataPoints);
    void AddDataPoint(T dataPoint);
    T GetMean();

private:
    int numDataPoints_;
    std::vector<T> data_;
    int index_;
    T sum_;
};

template <typename T>
SimpleMovingAverage<T>::SimpleMovingAverage(int numDataPoints)
    : numDataPoints_(numDataPoints), index_(0), sum_(0)
{
    if (!(numDataPoints > 0)) {
        throw std::invalid_argument("numDataPoints must be greater than zero");
    }
    data_ = std::vector<T>(numDataPoints, 0);
}

template <typename T>
void SimpleMovingAverage<T>::AddDataPoint(T dataPoint)
{
    sum_ -= data_[index_];
    sum_ += dataPoint;
    data_[index_] = dataPoint;
    ++index_;
    if (index_ == numDataPoints_) {
        index_ = 0;
    }
}

template <typename T>
T SimpleMovingAverage<T>::GetMean()
{
    return sum_ / numDataPoints_;
}
} // namespace Procdraw

#endif
