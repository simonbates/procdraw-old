#include "procdraw/interpreter/prettyprinter.h"
#include "procdraw/interpreter/lisp_interpreter.h"

namespace procdraw {

    PrettyPrinterToken PrettyPrinterToken::Begin()
    {
        return PrettyPrinterToken(PrettyPrinterTokenType::Begin);
    }

    PrettyPrinterToken PrettyPrinterToken::End()
    {
        return PrettyPrinterToken(PrettyPrinterTokenType::End);
    }

    PrettyPrinterToken PrettyPrinterToken::String(const std::string &str)
    {
        return PrettyPrinterToken(str);
    }

    PrettyPrinterToken PrettyPrinterToken::Blank()
    {
        return PrettyPrinterToken(PrettyPrinterTokenType::Blank);
    }

    std::string PrettyPrinter::PrintToString(LispInterpreter *L, LispObjectPtr obj, int margin, int indent)
    {
        margin_ = margin;
        indentAmount_ = indent;
        delimiterStack_ = std::stack<int>();
        indentStack_ = std::stack<int>();
        outstr_.clear();
        space_ = margin;
        Scan(L, obj);
        return outstr_;
    }

    void PrettyPrinter::Scan(LispInterpreter *L, LispObjectPtr obj)
    {
        if (L->TypeOf(obj) == LispObjectType::Cons) {
            Emit(PrettyPrinterToken::Begin());
            Emit(PrettyPrinterToken::String("("));
            LispObjectPtr n = obj;
            while (!L->Null(n)) {
                Scan(L, L->Car(n));
                n = L->Cdr(n);
                if (!L->Null(n) && L->Atom(n)) {
                    Emit(PrettyPrinterToken::String(" . "));
                    Scan(L, n);
                    n = L->Nil;
                }
                if (!L->Null(n)) {
                    Emit(PrettyPrinterToken::Blank());
                }
            }
            Emit(PrettyPrinterToken::String(")"));
            Emit(PrettyPrinterToken::End());
        }
        else {
            Emit(PrettyPrinterToken::String(L->PrintToString(obj)));
        }
    }

    // The pretty-printing algorithm is based on the "Inefficient But
    // Simple Algorithm" from "Prettyprinting" by Derek Oppen.
    //
    // Resources:
    //
    // - Ira Goldstein "Pretty-printing, converting list to linear structure"
    //   Artificial Intelligence Laboratory Memo No. 279, M.I.T., February 1973
    //   http://hdl.handle.net/1721.1/5797
    //
    // - Derek Oppen "Prettyprinting"
    //   ACM Transactions on Programming Languages and Systems
    //   Volume 2 Issue 4, October 1980
    //   http://dl.acm.org/citation.cfm?id=357114.357115
    //
    // - Richard C. Waters "XP. A Common Lisp Pretty Printing System"
    //   Artificial Intelligence Laboratory Memo No. 1102a, M.I.T., August 1989
    //   http://hdl.handle.net/1721.1/6504
    //
    // - PicoLisp Pretty-Print
    //   http://picolisp.com/wiki/?prettyPrint

    void PrettyPrinter::Emit(PrettyPrinterToken token)
    {
        switch (token.Type) {
        case PrettyPrinterTokenType::Begin:
            if (delimiterStack_.empty()) {
                stream_.clear();
                streamCharLen_ = 0;
            }
            token.Size = -streamCharLen_;
            stream_.push_back(token);
            delimiterStack_.push(stream_.size() - 1);
            break;
        case PrettyPrinterTokenType::End:
        {
            token.Size = 0;
            stream_.push_back(token);
            int x = delimiterStack_.top();
            delimiterStack_.pop();
            stream_.at(x).Size = streamCharLen_ + stream_.at(x).Size;
            if (stream_.at(x).Type == PrettyPrinterTokenType::Blank) {
                x = delimiterStack_.top();
                delimiterStack_.pop();
                stream_.at(x).Size = streamCharLen_ + stream_.at(x).Size;
            }
            if (delimiterStack_.empty()) {
                for (auto t : stream_) {
                    Print(t, t.Size);
                }
            }
            break;
        }
        case PrettyPrinterTokenType::Blank:
        {
            int x = delimiterStack_.top();
            if (stream_.at(x).Type == PrettyPrinterTokenType::Blank) {
                delimiterStack_.pop();
                stream_.at(x).Size = streamCharLen_ + stream_.at(x).Size;
            }
            token.Size = -streamCharLen_;
            stream_.push_back(token);
            delimiterStack_.push(stream_.size() - 1);
            ++streamCharLen_;
            break;
        }
        case PrettyPrinterTokenType::String:
            if (delimiterStack_.empty()) {
                Print(token, token.Str.length());
            }
            else {
                stream_.push_back(token);
                streamCharLen_ += token.Str.length();
            }
            break;
        }
    }

    void PrettyPrinter::Print(PrettyPrinterToken token, int len)
    {
        switch (token.Type) {
        case PrettyPrinterTokenType::String:
            outstr_.append(token.Str);
            space_ -= len;
            break;
        case PrettyPrinterTokenType::Begin:
            indentStack_.push(space_);
            break;
        case PrettyPrinterTokenType::End:
            indentStack_.pop();
            break;
        case PrettyPrinterTokenType::Blank:
            if (len > space_) {
                space_ = indentStack_.top() - indentAmount_;
                outstr_.append("\n");
                outstr_.append(std::string(margin_ - space_, ' '));
            }
            else {
                outstr_.append(" ");
                --space_;
            }
            break;
        }
    }

}
