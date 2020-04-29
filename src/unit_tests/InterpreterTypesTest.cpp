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
#include <functional>
#include <gtest/gtest.h>
#include <vector>

using namespace Procdraw;

class InterpreterTypesTest : public ::testing::Test {
protected:
    void ForAllTypesExcept(ObjectType type,
                           const std::function<void(Object)>& callback)
    {
        for (const auto& obj : allTypesObjs_) {
            if (obj.Type() != type) {
                callback(obj);
            }
        }
    };

    const Object trueObj_{true};
    const Object falseObj_{false};
    const Object cfunctionHandleObj_ = Object::MakeCFunctionHandle(10);
    const Object integerObj_{42};
    const Object listPtrObj_ = Object::EmptyList();
    const Object noneObj_ = Object::None();
    const Object symbolHandleObj_ = Object::MakeSymbolHandle(20);
    const std::vector<Object> allTypesObjs_{
        trueObj_,
        cfunctionHandleObj_,
        integerObj_,
        listPtrObj_,
        noneObj_,
        symbolHandleObj_};
};

TEST_F(InterpreterTypesTest, Boolean)
{
    EXPECT_EQ(ObjectType::Boolean, trueObj_.Type());
    EXPECT_TRUE(trueObj_.GetBoolean());

    EXPECT_EQ(ObjectType::Boolean, falseObj_.Type());
    EXPECT_FALSE(falseObj_.GetBoolean());

    int numOtherTypes{0};
    ForAllTypesExcept(ObjectType::Boolean, [&numOtherTypes](const Object& obj) {
        EXPECT_THROW(obj.GetBoolean(), BadObjectAccess);
        ++numOtherTypes;
    });
    EXPECT_EQ(5, numOtherTypes);
}

TEST_F(InterpreterTypesTest, CFunctionHandle)
{
    EXPECT_EQ(ObjectType::CFunctionHandle, cfunctionHandleObj_.Type());
    EXPECT_EQ(10, cfunctionHandleObj_.GetCFunctionHandle());

    int numOtherTypes{0};
    ForAllTypesExcept(ObjectType::CFunctionHandle, [&numOtherTypes](const Object& obj) {
        EXPECT_THROW(obj.GetCFunctionHandle(), BadObjectAccess);
        ++numOtherTypes;
    });
    EXPECT_EQ(5, numOtherTypes);
}

TEST_F(InterpreterTypesTest, Integer)
{
    EXPECT_EQ(ObjectType::Integer, integerObj_.Type());
    EXPECT_EQ(42, integerObj_.GetInteger());

    int numOtherTypes{0};
    ForAllTypesExcept(ObjectType::Integer, [&numOtherTypes](const Object& obj) {
        EXPECT_THROW(obj.GetInteger(), BadObjectAccess);
        ++numOtherTypes;
    });
    EXPECT_EQ(5, numOtherTypes);
}

TEST_F(InterpreterTypesTest, ListPtr)
{
    EXPECT_EQ(ObjectType::ListPtr, listPtrObj_.Type());
    EXPECT_EQ(nullptr, listPtrObj_.GetListPtr());

    int numOtherTypes{0};
    ForAllTypesExcept(ObjectType::ListPtr, [&numOtherTypes](const Object& obj) {
        EXPECT_THROW(obj.GetListPtr(), BadObjectAccess);
        ++numOtherTypes;
    });
    EXPECT_EQ(5, numOtherTypes);
}

TEST_F(InterpreterTypesTest, None)
{
    EXPECT_EQ(ObjectType::None, noneObj_.Type());
}

TEST_F(InterpreterTypesTest, SymbolHandle)
{
    EXPECT_EQ(ObjectType::SymbolHandle, symbolHandleObj_.Type());
    EXPECT_EQ(20, symbolHandleObj_.GetSymbolHandle());

    int numOtherTypes{0};
    ForAllTypesExcept(ObjectType::SymbolHandle, [&numOtherTypes](const Object& obj) {
        EXPECT_THROW(obj.GetSymbolHandle(), BadObjectAccess);
        ++numOtherTypes;
    });
    EXPECT_EQ(5, numOtherTypes);
}
