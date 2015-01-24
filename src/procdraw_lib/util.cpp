#include "util.h"
#include <cmath>

namespace procdraw {

    double MapRange(double a1, double a2, double b1, double b2, double s)
    {
        return b1 + ((s - a1) * (b2 - b1)) / (a2 - a1);
    }

    double WrapRange(double min, double max, double x)
    {
        return x - floor((x - min) / (max - min)) * (max - min);
    }

}
