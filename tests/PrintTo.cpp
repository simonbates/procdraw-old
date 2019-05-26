/* Copyright (c) 2019 Simon Bates
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "pch.h"
#include "PrintTo.h"

namespace Procdraw {

void PrintTo(const ObjType& type, std::ostream* os)
{
    switch (type) {
    case Procdraw::ObjType::Boolean:
        *os << "Boolean";
        break;
    case Procdraw::ObjType::ConsPtr:
        *os << "ConsPtr";
        break;
    case Procdraw::ObjType::Expr:
        *os << "Expr";
        break;
    case Procdraw::ObjType::Fsubr:
        *os << "Fsubr";
        break;
    case Procdraw::ObjType::Null:
        *os << "Null";
        break;
    case Procdraw::ObjType::Real:
        *os << "Real";
        break;
    case Procdraw::ObjType::Subr:
        *os <<"Subr";
        break;
    case Procdraw::ObjType::SymbolPtr:
        *os << "SymbolPtr";
        break;
    default:
        *os << "*UNKNOWN*";
        break;
    }
}

}
