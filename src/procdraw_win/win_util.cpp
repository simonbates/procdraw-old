#include "stdafx.h"
#include "win_util.h"

namespace procdraw {

    bool IsFirstKeydown(LPARAM lParam)
    {
        return !(lParam & (1 << 30));
    }

    std::vector<WCHAR> Utf8ToUtf16(const std::string &utf8str)
    {
        auto numWchars = MultiByteToWideChar(CP_UTF8, 0, utf8str.c_str(), -1, nullptr, 0);
        std::vector<WCHAR> utf16str(numWchars);
        MultiByteToWideChar(CP_UTF8, 0, utf8str.c_str(), -1, utf16str.data(), numWchars);
        return utf16str;
    }

    std::string Utf16ToUtf8(LPCWSTR utf16str, int count)
    {
        auto numChars = WideCharToMultiByte(CP_UTF8, 0, utf16str, count, nullptr, 0, nullptr, nullptr);
        std::vector<char> utf8str(numChars);
        WideCharToMultiByte(CP_UTF8, 0, utf16str, count, utf8str.data(), numChars, nullptr, nullptr);
        if (count == -1) {
            return std::string(utf8str.data());
        }
        else {
            return std::string(utf8str.data(), numChars);
        }
    }

    void TriangleNormal(DirectX::XMFLOAT3 *normal, const DirectX::XMFLOAT3 *vertex1,
                        const DirectX::XMFLOAT3 *vertex2, const DirectX::XMFLOAT3 *vertex3)
    {
        auto v1 = XMLoadFloat3(vertex1);
        auto v2 = XMLoadFloat3(vertex2);
        auto v3 = XMLoadFloat3(vertex3);
        auto n = DirectX::XMVector3Cross(DirectX::XMVectorSubtract(v2, v1), DirectX::XMVectorSubtract(v3, v1));
        XMStoreFloat3(normal, DirectX::XMVector3Normalize(n));
    }

}
