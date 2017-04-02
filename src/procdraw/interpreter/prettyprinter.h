#pragma once

#include "procdraw/interpreter/lisp_memory.h"
#include <stack>
#include <string>
#include <vector>

namespace procdraw {

class LispInterpreter;

enum class PrettyPrinterTokenType { Begin, End, String, Blank };

class PrettyPrinterToken {
public:
    PrettyPrinterTokenType Type;
    std::string Str;
    int Size = 0;
    int IndentAmount = 0;
    static PrettyPrinterToken Begin(int indentAmount);
    static PrettyPrinterToken End();
    static PrettyPrinterToken String(const std::string& str);
    static PrettyPrinterToken Blank();

private:
    explicit PrettyPrinterToken(PrettyPrinterTokenType t) : Type(t) {}
    explicit PrettyPrinterToken(const std::string& s)
        : Type(PrettyPrinterTokenType::String), Str(s), Size(s.length())
    {
    }
};

class PrettyPrinter {
public:
    std::string
    PrintToString(LispInterpreter* L, LispObjectPtr obj, int margin);

private:
    int indentAmount_ = 1;
    int lambdaIndentAmount_ = 2;
    int margin_;
    std::stack<int> blockStack_;
    std::stack<int> indentStack_;
    std::string outstr_;
    int printCol_;
    std::vector<PrettyPrinterToken> stream_;
    int streamCharLen_;
    std::vector<int> endedSinceBlank_;
    void Scan(LispInterpreter* L, LispObjectPtr obj);
    void ScanListContents(LispInterpreter* L, LispObjectPtr obj);
    void Emit(PrettyPrinterToken token);
    void Print(PrettyPrinterToken token);
};
}
