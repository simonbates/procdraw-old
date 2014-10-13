#pragma once

namespace procdraw {

    template <typename Type>
    Type Clamp(Type value, Type lower, Type upper)
    {
        if (value < lower) {
            return lower;
        }
        else if (value > upper) {
            return upper;
        }
        return value;
    }

}
