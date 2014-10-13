#pragma once

#include <string>

namespace procdraw {

    class LineBuffer {
    public:
        LineBuffer() :
            line_(L""), promptLen_(0) { }
        LineBuffer(const std::wstring &prompt) :
            line_(prompt), promptLen_(prompt.length()) { }
        void Clear(const std::wstring &prompt);
        std::wstring GetLine();
        std::wstring GetUserText();
        void Insert(wchar_t ch);
        void DeleteBack();
    private:
        std::wstring line_;
        int promptLen_;
    };

}
