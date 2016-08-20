#include "procdraw/interpreter/prettyprinter.h"
#include "procdraw/interpreter/lisp_interpreter.h"

static const int blockFits = -1;

namespace procdraw {

    PrettyPrinterToken PrettyPrinterToken::Begin()
    {
        return PrettyPrinterToken(PrettyPrinterTokenType::Begin, 0);
    }

    PrettyPrinterToken PrettyPrinterToken::End()
    {
        return PrettyPrinterToken(PrettyPrinterTokenType::End, 0);
    }

    PrettyPrinterToken PrettyPrinterToken::String(const std::string &str)
    {
        return PrettyPrinterToken(str);
    }

    PrettyPrinterToken PrettyPrinterToken::Blank()
    {
        return PrettyPrinterToken(PrettyPrinterTokenType::Blank, 1);
    }

    std::string PrettyPrinter::PrintToString(LispInterpreter *L, LispObjectPtr obj, int margin)
    {
        margin_ = margin;
        blockStack_ = std::stack<int>();
        indentStack_ = std::stack<int>();
        outstr_.clear();
        printCol_ = 0;
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
            if (blockStack_.empty()) {
                stream_.clear();
                streamCharLen_ = 0;
                endedSinceBlank_.clear();
            }
            token.Size = -streamCharLen_;
            stream_.push_back(token);
            blockStack_.push(stream_.size() - 1);
            break;
        case PrettyPrinterTokenType::End:
        {
            stream_.push_back(token);
            int x = blockStack_.top();
            blockStack_.pop();
            stream_.at(x).Size = streamCharLen_ + stream_.at(x).Size;
            endedSinceBlank_.push_back(x);
            if (blockStack_.empty()) {
                for (auto t : stream_) {
                    Print(t);
                }
            }
            break;
        }
        case PrettyPrinterTokenType::Blank:
        {
            stream_.push_back(token);
            streamCharLen_ += token.Size;
            endedSinceBlank_.clear();
            break;
        }
        case PrettyPrinterTokenType::String:
            if (blockStack_.empty()) {
                Print(token);
            }
            else {
                stream_.push_back(token);
                streamCharLen_ += token.Size;
                for (auto x : endedSinceBlank_) {
                    // Extend the sizes of any blocks that have ended without
                    // a blank occuring
                    stream_.at(x).Size += token.Size;
                }
            }
            break;
        }
    }

    void PrettyPrinter::Print(PrettyPrinterToken token)
    {
        switch (token.Type) {
        case PrettyPrinterTokenType::String:
            outstr_.append(token.Str);
            printCol_ += token.Size;
            break;
        case PrettyPrinterTokenType::Begin:
            if (token.Size <= (margin_ - printCol_)) {
                indentStack_.push(blockFits);
            }
            else {
                indentStack_.push(printCol_);
            }
            break;
        case PrettyPrinterTokenType::End:
            indentStack_.pop();
            break;
        case PrettyPrinterTokenType::Blank:
            if (indentStack_.top() == blockFits) {
                outstr_.append(" ");
                ++printCol_;
            }
            else {
                printCol_ = indentStack_.top() + indentAmount_;
                outstr_.append("\n");
                outstr_.append(std::string(printCol_, ' '));
            }
            break;
        }
    }

}
