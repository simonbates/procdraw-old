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

#include "ProcdrawDocs.h"
#include <pugixml.hpp>
#include <stdexcept>
#include <string>

namespace Procdraw::Tests {

ProcdrawDocs::ProcdrawDocs(const char* filename)
{
    auto result = doc.load_file(filename);
    if (!result) {
        throw std::invalid_argument(std::string("Error loading file: ") + filename);
    }
}

FunctionDocIterator ProcdrawDocs::FunctionDocs() const
{
    return FunctionDocIterator(doc);
}

FunctionDocIterator::FunctionDocIterator(const pugi::xml_document& doc)
{
    nodes = doc.select_nodes("//function");
    iter = nodes.begin();
    end = nodes.end();
}

bool FunctionDocIterator::HasNext()
{
    return iter != end;
}

FunctionDoc FunctionDocIterator::Next()
{
    pugi::xpath_node node = *iter;
    std::vector<FunctionExample> examples;
    for (auto example : node.node().child("examples").children("ex")) {
        examples.emplace_back(example.attribute("expr").value(),
                              example.attribute("value").value());
    }
    ++iter;
    return FunctionDoc(node.node().attribute("name").value(), examples);
}

} // namespace Procdraw::Tests
