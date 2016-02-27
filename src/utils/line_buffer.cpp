#include "line_buffer.h"

namespace procdraw {

    void LineBuffer::Clear(const std::string &prompt)
    {
        line_ = prompt;
        promptLen_ = prompt.length();
        cursorPos_ = promptLen_;
    }

    std::string LineBuffer::GetLine()
    {
        return line_;
    }

    std::string LineBuffer::GetUserText()
    {
        return line_.substr(promptLen_, line_.length() - promptLen_);
    }

    int LineBuffer::GetCursorPos()
    {
        return cursorPos_;
    }

    void LineBuffer::ForwardChar()
    {
        if (cursorPos_ < line_.length()) {
            ++cursorPos_;
        }
    }

    void LineBuffer::BackwardChar()
    {
        if (cursorPos_ > promptLen_) {
            --cursorPos_;
        }
    }

    void LineBuffer::Insert(char ch)
    {
        line_.insert(cursorPos_, 1, ch);
        ++cursorPos_;
    }

    void LineBuffer::DeleteBack()
    {
        if (cursorPos_ > promptLen_) {
            line_.erase(cursorPos_ - 1, 1);
            --cursorPos_;
        }
    }

}
