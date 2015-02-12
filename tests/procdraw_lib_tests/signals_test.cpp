#include "signals.h"
#include "lisp_interpreter.h"
#include "catch.hpp"

TEST_CASE("Signals")
{

    procdraw::LispInterpreter L;
    procdraw::RegisterSignals(&L);

    L.Eval(L.Read("(setq step-incr (lambda (self) (put self 'val1 (+ (get self 'val1) (get self 'incr)))))"));
    L.Eval(L.Read("(setq sig1 (make-signal step-incr))"));
    L.Eval(L.Read("(put sig1 'val1 1)"));
    L.Eval(L.Read("(put sig1 'incr 2)"));

    SECTION("step") {
        REQUIRE(L.NumVal(L.Eval(L.Read("(get sig1 'val1)"))) == 1);
        L.Eval(L.Read("('step sig1)"));
        REQUIRE(L.NumVal(L.Eval(L.Read("(get sig1 'val1)"))) == 3);
        L.Eval(L.Read("('step sig1)"));
        REQUIRE(L.NumVal(L.Eval(L.Read("(get sig1 'val1)"))) == 5);
    }

    SECTION("sigval") {
        REQUIRE(L.NumVal(L.Eval(L.Read("(sigval sig1)"))) == 3);
        REQUIRE(L.NumVal(L.Eval(L.Read("(sigval sig1)"))) == 3);
        L.Eval(L.Read("(clear-stepped-signals)"));
        REQUIRE(L.NumVal(L.Eval(L.Read("(sigval sig1)"))) == 5);
    }

    SECTION("=>") {
        // sig1        sig2
        // incr  val1  incr val1
        // 2     1     0    1     before
        // 2     3     3    4     step 1
        // 2     5     5    9     step 2
        L.Eval(L.Read("(setq sig2 (make-signal step-incr))"));
        L.Eval(L.Read("(put sig2 'val1 1)"));
        L.Eval(L.Read("(put sig2 'incr 0)"));
        L.Eval(L.Read("(=> sig1 sig2 'incr)"));
        REQUIRE(L.NumVal(L.Eval(L.Read("(sigval sig2)"))) == 4);
        REQUIRE(L.NumVal(L.Eval(L.Read("(sigval sig2)"))) == 4);
        REQUIRE(L.NumVal(L.Eval(L.Read("(sigval sig1)"))) == 3);
        REQUIRE(L.NumVal(L.Eval(L.Read("(sigval sig1)"))) == 3);
        L.Eval(L.Read("(clear-stepped-signals)"));
        REQUIRE(L.NumVal(L.Eval(L.Read("(sigval sig2)"))) == 9);
        REQUIRE(L.NumVal(L.Eval(L.Read("(sigval sig1)"))) == 5);
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
