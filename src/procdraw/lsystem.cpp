#include "procdraw/lsystem.h"
#include <string>

namespace procdraw {

std::string Lsystem::GetString() { return current_; }

void Lsystem::Next()
{
    std::string next;
    for (auto c : current_) {
        auto iter = rules_.find(c);
        if (iter != rules_.end()) {
            next += iter->second;
        } else {
            next += c;
        }
    }
    current_ = next;
}
}
