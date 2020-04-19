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

#include <exception>
#include <memory>
#include <variant>

namespace Procdraw {

enum class ObjectType {
    Boolean,
    CFunctionHandle,
    Integer,
    ListPtr,
    None,
    SymbolHandle
};

using CFunctionHandle = size_t;
using SymbolHandle = size_t;

class ListNode;

using ListPtr = std::shared_ptr<ListNode>;

class BadObjectAccess : public std::exception {
public:
    const char* what() const override { return "Bad Object Access"; }
};

class Object {
public:
    Object(bool val);
    Object(int val);
    Object(ListPtr val);
    Object(const Object& o);
    Object& operator=(const Object& o);
    ~Object();
    static Object EmptyList();
    static Object MakeCFunctionHandle(CFunctionHandle handle);
    static Object MakeSymbolHandle(SymbolHandle handle);
    static Object None();
    ObjectType Type() const;
    bool GetBoolean() const;
    CFunctionHandle GetCFunctionHandle() const;
    int GetInteger() const;
    ListPtr GetListPtr() const;
    SymbolHandle GetSymbolHandle() const;

private:
    ObjectType type;
    union {
        bool booleanVal;
        CFunctionHandle cfunctionHandleVal;
        int integerVal;
        ListPtr listPtrVal;
        SymbolHandle symbolHandleVal;
    };
    Object(ObjectType type)
        : type(type) {}
};

class ListNode {
public:
    ListNode(Object first, ListPtr rest)
        : first(first), rest(rest) {}
    Object First()
    {
        return first;
    }
    void SetFirst(Object obj)
    {
        first = obj;
    }
    ListPtr Rest()
    {
        return rest;
    }
    void SetRest(ListPtr lst)
    {
        rest = lst;
    }

private:
    Object first;
    ListPtr rest;
};

inline ListPtr Cons(Object first, ListPtr rest)
{
    return std::make_shared<ListNode>(first, rest);
}

inline Object::Object(bool val)
    : type(ObjectType::Boolean), booleanVal(val) {}

inline Object::Object(int val)
    : type(ObjectType::Integer), integerVal(val) {}

inline Object::Object(ListPtr val)
    : type(ObjectType::ListPtr), listPtrVal(val) {}

inline Object::Object(const Object& o)
    : type(o.type)
{
    switch (o.type) {
    case ObjectType::Boolean:
        booleanVal = o.booleanVal;
        break;
    case ObjectType::CFunctionHandle:
        cfunctionHandleVal = o.cfunctionHandleVal;
        break;
    case ObjectType::Integer:
        integerVal = o.integerVal;
        break;
    case ObjectType::ListPtr:
        new (&listPtrVal) ListPtr(o.listPtrVal);
        break;
    case ObjectType::SymbolHandle:
        symbolHandleVal = o.symbolHandleVal;
        break;
    }
}

inline Object& Object::operator=(const Object& o)
{
    if (type == ObjectType::ListPtr && o.type == ObjectType::ListPtr) {
        listPtrVal = o.listPtrVal;
        return *this;
    }

    if (type == ObjectType::ListPtr) {
        listPtrVal.~ListPtr();
    }

    switch (o.type) {
    case ObjectType::Boolean:
        booleanVal = o.booleanVal;
        break;
    case ObjectType::CFunctionHandle:
        cfunctionHandleVal = o.cfunctionHandleVal;
        break;
    case ObjectType::Integer:
        integerVal = o.integerVal;
        break;
    case ObjectType::ListPtr:
        new (&listPtrVal) ListPtr(o.listPtrVal);
        break;
    case ObjectType::SymbolHandle:
        symbolHandleVal = o.symbolHandleVal;
        break;
    }

    type = o.type;
    return *this;
}

inline Object::~Object()
{
    if (type == ObjectType::ListPtr) {
        listPtrVal.~ListPtr();
    }
}

inline Object Object::EmptyList()
{
    Object obj{ObjectType::ListPtr};
    new (&obj.listPtrVal) ListPtr(nullptr);
    return obj;
}

inline Object Object::MakeCFunctionHandle(CFunctionHandle handle)
{
    Object obj{ObjectType::CFunctionHandle};
    obj.cfunctionHandleVal = handle;
    return obj;
}

inline Object Object::MakeSymbolHandle(SymbolHandle handle)
{
    Object obj{ObjectType::SymbolHandle};
    obj.symbolHandleVal = handle;
    return obj;
}

inline Object Object::None()
{
    return Object{ObjectType::None};
}

inline ObjectType Object::Type() const
{
    return type;
}

inline bool Object::GetBoolean() const
{
    if (type != ObjectType::Boolean) {
        throw BadObjectAccess{};
    }
    return booleanVal;
}

inline CFunctionHandle Object::GetCFunctionHandle() const
{
    if (type != ObjectType::CFunctionHandle) {
        throw BadObjectAccess{};
    }
    return cfunctionHandleVal;
}

inline int Object::GetInteger() const
{
    if (type != ObjectType::Integer) {
        throw BadObjectAccess{};
    }
    return integerVal;
}

inline ListPtr Object::GetListPtr() const
{
    if (type != ObjectType::ListPtr) {
        throw BadObjectAccess{};
    }
    return listPtrVal;
}

inline SymbolHandle Object::GetSymbolHandle() const
{
    if (type != ObjectType::SymbolHandle) {
        throw BadObjectAccess{};
    }
    return symbolHandleVal;
}

} // namespace Procdraw
