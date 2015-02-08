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

    SECTION("saw") {
        L.Eval(L.Read("(setq saw1 (saw))"));
        REQUIRE(L.NumVal(L.Eval(L.Read("(get saw1 'freq)"))) == 0);
        REQUIRE(L.NumVal(L.Eval(L.Read("(get saw1 'val1)"))) == 0);
        REQUIRE(L.NumVal(L.Eval(L.Read("(sigval saw1)"))) == 0);

        L.Eval(L.Read("(put saw1 'freq (/ 3 8))"));
        L.Eval(L.Read("(clear-stepped-signals)"));
        REQUIRE(L.NumVal(L.Eval(L.Read("(sigval saw1)"))) == 0.375);
        L.Eval(L.Read("(clear-stepped-signals)"));
        REQUIRE(L.NumVal(L.Eval(L.Read("(sigval saw1)"))) == 0.75);
        L.Eval(L.Read("(clear-stepped-signals)"));
        REQUIRE(L.NumVal(L.Eval(L.Read("(sigval saw1)"))) == 0.125);

        L.Eval(L.Read("(put saw1 'freq (/ -5 8))"));
        L.Eval(L.Read("(clear-stepped-signals)"));
        REQUIRE(L.NumVal(L.Eval(L.Read("(sigval saw1)"))) == 0.5);
        L.Eval(L.Read("(clear-stepped-signals)"));
        REQUIRE(L.NumVal(L.Eval(L.Read("(sigval saw1)"))) == 0.875);
    }

}
