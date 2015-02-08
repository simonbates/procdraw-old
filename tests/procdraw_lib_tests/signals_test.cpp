#include "signals.h"
#include "lisp_interpreter.h"
#include "catch.hpp"

TEST_CASE("Signals") {

    procdraw::LispInterpreter L;
    procdraw::RegisterSignals(&L);

    auto exp = R"(
            (setq s (make-signal (lambda (self)
              (put self 'val1 (+ (get self 'val1) (get self 'incr))))))
        )";
    L.Eval(L.Read(exp));
    L.Eval(L.Read("(put s 'val1 1)"));
    L.Eval(L.Read("(put s 'incr 2)"));

    SECTION("step") {
        REQUIRE(L.NumVal(L.Eval(L.Read("(get s 'val1)"))) == 1);
        L.Eval(L.Read("('step s)"));
        REQUIRE(L.NumVal(L.Eval(L.Read("(get s 'val1)"))) == 3);
        L.Eval(L.Read("('step s)"));
        REQUIRE(L.NumVal(L.Eval(L.Read("(get s 'val1)"))) == 5);
    }

    SECTION("sigval") {
        REQUIRE(L.NumVal(L.Eval(L.Read("(sigval s)"))) == 3);
        REQUIRE(L.NumVal(L.Eval(L.Read("(sigval s)"))) == 3);
        L.Eval(L.Read("(clear-stepped-signals)"));
        REQUIRE(L.NumVal(L.Eval(L.Read("(sigval s)"))) == 5);
    }

}
