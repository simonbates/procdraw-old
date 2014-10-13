#include "line_buffer.h"

namespace procdraw {

    void LineBuffer::Clear(const std::wstring &prompt)
    {
        line_ = prompt;
        promptLen_ = prompt.length();
    }

    std::wstring LineBuffer::GetLine()
    {
        return line_;
    }

    std::wstring LineBuffer::GetUserText()
    {
        return line_.substr(promptLen_, line_.length() - promptLen_);
    }

    void LineBuffer::Insert(wchar_t ch)
    {
        line_.push_back(ch);
    }

    void LineBuffer::DeleteBack()
    {
        if (line_.length() > promptLen_) {
            line_.pop_back();
        }
    }

}
