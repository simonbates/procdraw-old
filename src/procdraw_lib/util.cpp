#include "util.h"
#include <cmath>

namespace procdraw {

    double Lerp(double start, double stop, double val)
    {
        return (1 - val) * start + val * stop;
    }

    double MapRange(double start1, double stop1, double start2, double stop2, double val)
    {
        return start2 + ((val - start1) * (stop2 - start2)) / (stop1 - start1);
    }

    double Norm(double start, double stop, double val)
    {
        return (val - start) / (stop - start);
    }

    double Wrap(double start, double stop, double val)
    {
        return val - floor((val - start) / (stop - start)) * (stop - start);
    }

}
