#pragma once

#include <string>

namespace procdraw {

class LineBuffer {
public:
    LineBuffer()
        : line_("")
        , promptLen_(0)
        , cursorPos_(0)
    {
    }
    explicit LineBuffer(const std::string& prompt)
        : line_(prompt)
        , promptLen_(prompt.length())
        , cursorPos_(promptLen_)
    {
    }
    void Clear(const std::string& prompt);
    std::string GetLine();
    std::string GetUserText();
    int GetCursorPos();
    void ForwardChar();
    void BackwardChar();
    void Insert(char ch);
    void DeleteBack();

private:
    std::string line_;
    unsigned int promptLen_;
    unsigned int cursorPos_;
};
}
