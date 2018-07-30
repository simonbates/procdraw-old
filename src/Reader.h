/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#pragma once

#include <sstream>
#include <string>

namespace Procdraw {

    class Interpreter;

    enum class ReaderTokenType {
        Dot,
        LParen,
        Real,
        RParen,
        Symbol,
        EndOfInput,
        Undefined
    };

    class Reader {
    public:
        Reader(Interpreter* interpreter, const std::string& str);
        void Read();
    private:
        Interpreter *interpreter_;
        std::istringstream input_;
        int ch_;
        ReaderTokenType token_;
        bool boolVal_;
        double realVal_;
        std::string symbolName_;
        void GetCh();
        void GetToken();
        void GetReal();
        void GetSymbol();
        void Expect(ReaderTokenType t);
        void ReadCons();
    };

}
