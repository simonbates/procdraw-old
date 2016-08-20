#pragma once

#include "lisp_memory.h"
#include <stack>
#include <string>
#include <vector>

namespace procdraw {

    class LispInterpreter;

    enum class PrettyPrinterTokenType {
        Begin, End, String, Blank
    };

    class PrettyPrinterToken {
    public:
        PrettyPrinterTokenType Type;
        std::string Str;
        int Size;
        static PrettyPrinterToken Begin();
        static PrettyPrinterToken End();
        static PrettyPrinterToken String(const std::string &str);
        static PrettyPrinterToken Blank();
    private:
        PrettyPrinterToken(PrettyPrinterTokenType t, int size) : Type(t), Size(size) { }
        PrettyPrinterToken(const std::string &s) : Type(PrettyPrinterTokenType::String), Str(s), Size(s.length()) { }
    };

    class PrettyPrinter {
    public:
        std::string PrintToString(LispInterpreter *L, LispObjectPtr obj, int margin);
    private:
        int indentAmount_ = 1;
        int margin_;
        std::stack<int> blockStack_;
        std::stack<int> indentStack_;
        std::string outstr_;
        int printCol_;
        std::vector<PrettyPrinterToken> stream_;
        int streamCharLen_;
        std::vector<int> endedSinceBlank_;
        void Scan(LispInterpreter *L, LispObjectPtr obj);
        void Emit(PrettyPrinterToken token);
        void Print(PrettyPrinterToken token);
    };

}
