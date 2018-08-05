/* Copyright (c) 2018 Simon Bates
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#pragma once

namespace Microsoft {
namespace VisualStudio {
namespace CppUnitTestFramework {

template<>
static std::wstring ToString<Procdraw::ObjType>(const Procdraw::ObjType& type)
{
    switch (type) {
    case Procdraw::ObjType::Boolean:
        return L"Boolean";
    case Procdraw::ObjType::ConsPtr:
        return L"ConsPtr";
    case Procdraw::ObjType::Expr:
        return L"Expr";
    case Procdraw::ObjType::Fsubr:
        return L"Fsubr";
    case Procdraw::ObjType::Null:
        return L"Null";
    case Procdraw::ObjType::Real:
        return L"Real";
    case Procdraw::ObjType::Subr:
        return L"Subr";
    case Procdraw::ObjType::SymbolPtr:
        return L"SymbolPtr";
    default:
        return L"*UNKNOWN*";
    }
};

}
}
}
