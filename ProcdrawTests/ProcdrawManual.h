/* Copyright (c) 2018 Simon Bates
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#pragma once

#include <pugixml.hpp>
#include <string>
#include <vector>

namespace Procdraw {
namespace Tests {

class FunctionExample {
public:
    FunctionExample(const std::string& expr,
                    const std::string& value)
        : expr_(expr), value_(value){};
    std::string Expression() const
    {
        return expr_;
    };
    std::string Value() const
    {
        return value_;
    };

private:
    std::string expr_;
    std::string value_;
};

class FunctionDoc {
public:
    FunctionDoc(const std::string& name,
                const std::vector<FunctionExample>& examples)
        : name_(name), examples_(examples){};
    std::string Name() const
    {
        return name_;
    };
    std::vector<FunctionExample> Examples() const
    {
        return examples_;
    };

private:
    std::string name_;
    std::vector<FunctionExample> examples_;
};

class FunctionDocIterator {
public:
    FunctionDocIterator(const pugi::xml_document& doc);
    bool HasNext();
    FunctionDoc Next();

private:
    pugi::xpath_node_set nodes_;
    pugi::xpath_node_set::const_iterator iter_;
    pugi::xpath_node_set::const_iterator end_;
};

class ProcdrawManual {
public:
    ProcdrawManual(const char* filename);
    FunctionDocIterator FunctionDocs() const;

private:
    pugi::xml_document doc_;
};

} // namespace Tests
} // namespace Procdraw
