#pragma once

#include <comdef.h>
#include <string>
#include <vector>

namespace procdraw {

    inline void ThrowOnFail(HRESULT hr)
    {
        if (FAILED(hr)) {
            throw _com_error(hr);
        }
    }

    bool IsFirstKeydown(LPARAM lParam);

    std::vector<WCHAR> Utf8ToUtf16(const std::string &utf8str);

    std::string Utf16ToUtf8(LPCWSTR utf16str, int count);

}
