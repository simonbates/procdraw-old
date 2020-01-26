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

#include "../lib/InterpreterTypes.h"
#include <catch.hpp>
#include <functional>

using namespace Procdraw;

TEST_CASE("Interpreter Types")
{
    Object trueObj{true};
    Object falseObj{false};
    Object cfunctionHandleObj = Object::MakeCFunctionHandle(10);
    Object integerObj{42};
    Object listPtrObj = Object::EmptyList();
    Object noneObj = Object::None();
    Object symbolHandleObj = Object::MakeSymbolHandle(20);

    auto allTypesObjs = {
        trueObj,
        cfunctionHandleObj,
        integerObj,
        listPtrObj,
        noneObj,
        symbolHandleObj};

    auto forAllTypesExcept = [allTypesObjs](
                                 ObjectType type,
                                 const std::function<void(Object)>& callback) {
        for (const auto& obj : allTypesObjs) {
            if (obj.Type() != type) {
                callback(obj);
            }
        }
    };

    SECTION("Boolean")
    {
        REQUIRE(trueObj.Type() == ObjectType::Boolean);
        REQUIRE(trueObj.GetBoolean());

        REQUIRE(falseObj.Type() == ObjectType::Boolean);
        REQUIRE_FALSE(falseObj.GetBoolean());

        forAllTypesExcept(ObjectType::Boolean, [](const Object& obj) {
            REQUIRE_THROWS_AS(obj.GetBoolean(), BadObjectAccess);
        });
    }

    SECTION("CFunctionHandle")
    {
        REQUIRE(cfunctionHandleObj.Type() == ObjectType::CFunctionHandle);
        REQUIRE(cfunctionHandleObj.GetCFunctionHandle() == 10);

        forAllTypesExcept(ObjectType::CFunctionHandle, [](const Object& obj) {
            REQUIRE_THROWS_AS(obj.GetCFunctionHandle(), BadObjectAccess);
        });
    }

    SECTION("Integer")
    {
        REQUIRE(integerObj.Type() == ObjectType::Integer);
        REQUIRE(integerObj.GetInteger() == 42);

        forAllTypesExcept(ObjectType::Integer, [](const Object& obj) {
            REQUIRE_THROWS_AS(obj.GetInteger(), BadObjectAccess);
        });
    }

    SECTION("ListPtr")
    {
        REQUIRE(listPtrObj.Type() == ObjectType::ListPtr);
        REQUIRE(listPtrObj.GetListPtr() == nullptr);

        forAllTypesExcept(ObjectType::ListPtr, [](const Object& obj) {
            REQUIRE_THROWS_AS(obj.GetListPtr(), BadObjectAccess);
        });
    }

    SECTION("None")
    {
        REQUIRE(noneObj.Type() == ObjectType::None);
    }

    SECTION("SymbolHandle")
    {
        REQUIRE(symbolHandleObj.Type() == ObjectType::SymbolHandle);
        REQUIRE(symbolHandleObj.GetSymbolHandle() == 20);

        forAllTypesExcept(ObjectType::SymbolHandle, [](const Object& obj) {
            REQUIRE_THROWS_AS(obj.GetSymbolHandle(), BadObjectAccess);
        });
    }
}
