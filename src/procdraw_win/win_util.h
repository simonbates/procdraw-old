#pragma once

#include <comdef.h>

namespace procdraw {

    inline void ThrowOnFail(HRESULT hr)
    {
        if (FAILED(hr)) {
            throw _com_error(hr);
        }
    }

    bool IsFirstKeydown(LPARAM lParam);

}
