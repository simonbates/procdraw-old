/* Copyright (c) 2018, 2019 Simon Bates
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "pch.h"
#include "ProcdrawManual.h"
#include <pugixml.hpp>
#include <stdexcept>
#include <string>

namespace Procdraw {
namespace Tests {

ProcdrawManual::ProcdrawManual(const char* filename)
{
    auto result = doc_.load_file(filename);
    if (!result) {
        throw std::invalid_argument(std::string("Error loading file: ") + filename);
    }
}

FunctionDocIterator ProcdrawManual::FunctionDocs() const
{
    return FunctionDocIterator(doc_);
}

FunctionDocIterator::FunctionDocIterator(const pugi::xml_document& doc)
{
    nodes_ = doc.select_nodes("//function-doc");
    iter_ = nodes_.begin();
    end_ = nodes_.end();
}

bool FunctionDocIterator::HasNext()
{
    return iter_ != end_;
}

FunctionDoc FunctionDocIterator::Next()
{
    pugi::xpath_node node = *iter_;
    std::vector<FunctionExample> examples;
    for (auto example : node.node().child("examples").children("ex")) {
        examples.emplace_back(example.attribute("expr").value(),
                              example.attribute("value").value());
    }
    ++iter_;
    return FunctionDoc(node.node().attribute("name").value(), examples);
}

} // namespace Tests
} // namespace Procdraw
