#pragma once

namespace Microsoft {
namespace VisualStudio {
namespace CppUnitTestFramework {

template<>
static std::wstring ToString<procdraw::ObjType>(const procdraw::ObjType& type)
{
    switch (type) {
    case procdraw::ObjType::Boolean:
        return L"Boolean";
    case procdraw::ObjType::ConsPtr:
        return L"ConsPtr";
    case procdraw::ObjType::Expr:
        return L"Expr";
    case procdraw::ObjType::Fsubr:
        return L"Fsubr";
    case procdraw::ObjType::Null:
        return L"Null";
    case procdraw::ObjType::Real:
        return L"Real";
    case procdraw::ObjType::Subr:
        return L"Subr";
    case procdraw::ObjType::SymbolPtr:
        return L"SymbolPtr";
    default:
        return L"*UNKNOWN*";
    }
};

}
}
}
