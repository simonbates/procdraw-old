#include "stdafx.h"
#include "win_util.h"

namespace procdraw {

    bool IsFirstKeydown(LPARAM lParam)
    {
        return !(lParam & (1 << 30));
    }

}
