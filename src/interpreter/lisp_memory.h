#pragma once

#include <memory>

namespace procdraw {

    class LispObject;
    typedef std::shared_ptr<LispObject> LispObjectPtr;

    bool LispObjectEq(const LispObjectPtr &x, const LispObjectPtr &y);

}
