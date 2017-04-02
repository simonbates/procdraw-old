#pragma once

#include <map>
#include <string>

namespace procdraw {

class Lsystem {
public:
    Lsystem(const std::string& start, const std::map<char, std::string>& rules)
        : current_(start), rules_(rules)
    {
    }
    std::string GetString();
    void Next();

private:
    std::string current_;
    std::map<char, std::string> rules_;
};
}
