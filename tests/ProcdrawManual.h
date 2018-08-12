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
    FunctionExample(const std::string& expression,
                    const std::string& value)
        : expression(expression), value(value){};
    std::string Expression() const
    {
        return expression;
    };
    std::string Value() const
    {
        return value;
    };

private:
    std::string expression;
    std::string value;
};

class FunctionDoc {
public:
    FunctionDoc(const std::string& name,
                const std::vector<FunctionExample>& examples)
        : name(name), examples(examples){};
    std::string Name() const
    {
        return name;
    };
    std::vector<FunctionExample> Examples() const
    {
        return examples;
    };

private:
    std::string name;
    std::vector<FunctionExample> examples;
};

class FunctionDocIterator {
public:
    FunctionDocIterator(const pugi::xml_document& doc);
    bool HasNext();
    FunctionDoc Next();

private:
    pugi::xpath_node_set nodes;
    pugi::xpath_node_set::const_iterator iter;
    pugi::xpath_node_set::const_iterator end;
};

class ProcdrawManual {
public:
    ProcdrawManual(const char* filename);
    FunctionDocIterator FunctionDocs() const;

private:
    pugi::xml_document doc;
};

} // namespace Tests
} // namespace Procdraw
