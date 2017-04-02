#pragma once

#include <stdexcept>
#include <vector>

namespace procdraw {

template <typename T> class SimpleMovingAverage {
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

template <typename T> void SimpleMovingAverage<T>::AddDataPoint(T dataPoint)
{
    sum_ -= data_[index_];
    sum_ += dataPoint;
    data_[index_] = dataPoint;
    ++index_;
    if (index_ == numDataPoints_) {
        index_ = 0;
    }
}

template <typename T> T SimpleMovingAverage<T>::GetMean()
{
    return sum_ / numDataPoints_;
}
}
