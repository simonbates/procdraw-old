#include "util.h"
#include <cmath>

namespace procdraw {

    double MapRange(double a1, double a2, double b1, double b2, double s)
    {
        return b1 + ((s - a1) * (b2 - b1)) / (a2 - a1);
    }

    double Wrap(double start, double stop, double val)
    {
        return val - floor((val - start) / (stop - start)) * (stop - start);
    }

}
