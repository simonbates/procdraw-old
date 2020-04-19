// Copyright 2020 Simon Bates
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <pugixml.hpp>
#include <string>
#include <vector>

namespace Procdraw::Tests {

class FunctionExample {
public:
    FunctionExample(const std::string& expr,
                    const std::string& value)
        : expr(expr), value(value){};
    std::string Expression() const
    {
        return expr;
    };
    std::string Value() const
    {
        return value;
    };

private:
    std::string expr;
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

class ProcdrawDocs {
public:
    ProcdrawDocs(const char* filename);
    FunctionDocIterator FunctionDocs() const;

private:
    pugi::xml_document doc;
};

} // namespace Procdraw::Tests
