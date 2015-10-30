#include "signals.h"
#include "lisp_interpreter.h"
#include "util.h"
#include "catch.hpp"

TEST_CASE("Signals")
{
    procdraw::LispInterpreter L;
    procdraw::RegisterSignals(&L);

    L.Eval(L.Read("(setq step-incr (lambda (self) (put-slot self 'out (+ (get-slot self 'out) (get-slot self 'incr)))))"));
    L.Eval(L.Read("(setq sig1 (make-signal step-incr))"));
    L.Eval(L.Read("(put-slot sig1 'out 1)"));
    L.Eval(L.Read("(put-slot sig1 'incr 2)"));
    REQUIRE(L.NumVal(L.Eval(L.Read("(get-slot sig1 'out)"))) == 1);
    REQUIRE(L.NumVal(L.Eval(L.Read("(get-slot sig1 'incr)"))) == 2);

    SECTION("sigval") {
        REQUIRE(L.NumVal(L.Eval(L.Read("(sigval sig1)"))) == 3);
        REQUIRE(L.NumVal(L.Eval(L.Read("(sigval sig1)"))) == 3);
        L.Eval(L.Read("(clear-stepped-signals)"));
        REQUIRE(L.NumVal(L.Eval(L.Read("(sigval sig1)"))) == 5);
    }

    SECTION("sigval with a null step") {
        L.Eval(L.Read("(setq null-step-signal (make-signal nil))"));
        L.Eval(L.Read("(put-slot null-step-signal 'out 2)"));
        REQUIRE(L.NumVal(L.Eval(L.Read("(sigval null-step-signal)"))) == 2);
    }

    SECTION("=> with single sigval") {
        // sig1        sig2
        // incr  out   incr  out
        // 2     1     0     1     before
        // 2     3     3     4     step 1
        // 2     5     5     9     step 2
        L.Eval(L.Read("(setq sig2 (make-signal step-incr))"));
        L.Eval(L.Read("(put-slot sig2 'out 1)"));
        L.Eval(L.Read("(put-slot sig2 'incr 0)"));
        L.Eval(L.Read("(=> '(sigval sig1) sig2 'incr)"));
        REQUIRE(L.NumVal(L.Eval(L.Read("(sigval sig2)"))) == 4);
        REQUIRE(L.NumVal(L.Eval(L.Read("(sigval sig2)"))) == 4);
        REQUIRE(L.NumVal(L.Eval(L.Read("(sigval sig1)"))) == 3);
        REQUIRE(L.NumVal(L.Eval(L.Read("(sigval sig1)"))) == 3);
        L.Eval(L.Read("(clear-stepped-signals)"));
        REQUIRE(L.NumVal(L.Eval(L.Read("(sigval sig2)"))) == 9);
        REQUIRE(L.NumVal(L.Eval(L.Read("(sigval sig1)"))) == 5);
    }

    SECTION("=> with expression of sigval") {
        // sig1        sig2
        // incr  out   incr  out
        // 2     1     0     1     before
        // 2     3     30    31    step 1
        // 2     5     50    81    step 2
        L.Eval(L.Read("(setq sig2 (make-signal step-incr))"));
        L.Eval(L.Read("(put-slot sig2 'out 1)"));
        L.Eval(L.Read("(put-slot sig2 'incr 0)"));
        L.Eval(L.Read("(=> '(* 10 (sigval sig1)) sig2 'incr)"));
        REQUIRE(L.NumVal(L.Eval(L.Read("(sigval sig2)"))) == 31);
        REQUIRE(L.NumVal(L.Eval(L.Read("(sigval sig2)"))) == 31);
        REQUIRE(L.NumVal(L.Eval(L.Read("(sigval sig1)"))) == 3);
        REQUIRE(L.NumVal(L.Eval(L.Read("(sigval sig1)"))) == 3);
        L.Eval(L.Read("(clear-stepped-signals)"));
        REQUIRE(L.NumVal(L.Eval(L.Read("(sigval sig2)"))) == 81);
        REQUIRE(L.NumVal(L.Eval(L.Read("(sigval sig1)"))) == 5);
    }

    SECTION("saw") {
        L.Eval(L.Read("(setq saw1 (saw))"));
        REQUIRE(L.NumVal(L.Eval(L.Read("(get-slot saw1 'freq)"))) == 0);
        REQUIRE(L.NumVal(L.Eval(L.Read("(get-slot saw1 'out)"))) == 0);
        REQUIRE(L.NumVal(L.Eval(L.Read("(sigval saw1)"))) == 0);

        L.Eval(L.Read("(put-slot saw1 'freq (/ 3 8))"));
        L.Eval(L.Read("(clear-stepped-signals)"));
        REQUIRE(L.NumVal(L.Eval(L.Read("(sigval saw1)"))) == 0.375);
        L.Eval(L.Read("(clear-stepped-signals)"));
        REQUIRE(L.NumVal(L.Eval(L.Read("(sigval saw1)"))) == 0.75);
        L.Eval(L.Read("(clear-stepped-signals)"));
        REQUIRE(L.NumVal(L.Eval(L.Read("(sigval saw1)"))) == 0.125);

        L.Eval(L.Read("(put-slot saw1 'freq (/ -5 8))"));
        L.Eval(L.Read("(clear-stepped-signals)"));
        REQUIRE(L.NumVal(L.Eval(L.Read("(sigval saw1)"))) == 0.5);
        L.Eval(L.Read("(clear-stepped-signals)"));
        REQUIRE(L.NumVal(L.Eval(L.Read("(sigval saw1)"))) == 0.875);
    }

    SECTION("tri") {
        L.Eval(L.Read("(setq tri1 (tri))"));
        REQUIRE(L.NumVal(L.Eval(L.Read("(get-slot tri1 'freq)"))) == 0);
        REQUIRE(L.NumVal(L.Eval(L.Read("(get-slot tri1 'out)"))) == 0);
        REQUIRE(L.NumVal(L.Eval(L.Read("(sigval tri1)"))) == 0);

        L.Eval(L.Read("(put-slot tri1 'freq (/ 4))"));
        L.Eval(L.Read("(clear-stepped-signals)"));
        REQUIRE(L.NumVal(L.Eval(L.Read("(sigval tri1)"))) == 0.5);
        L.Eval(L.Read("(clear-stepped-signals)"));
        REQUIRE(L.NumVal(L.Eval(L.Read("(sigval tri1)"))) == 1);
        L.Eval(L.Read("(clear-stepped-signals)"));
        REQUIRE(L.NumVal(L.Eval(L.Read("(sigval tri1)"))) == 0.5);
        L.Eval(L.Read("(clear-stepped-signals)"));
        REQUIRE(L.NumVal(L.Eval(L.Read("(sigval tri1)"))) == 0);
        L.Eval(L.Read("(clear-stepped-signals)"));
        REQUIRE(L.NumVal(L.Eval(L.Read("(sigval tri1)"))) == 0.5);

        L.Eval(L.Read("(put-slot tri1 'freq (/ -3 8))"));
        L.Eval(L.Read("(clear-stepped-signals)"));
        REQUIRE(L.NumVal(L.Eval(L.Read("(sigval tri1)"))) == 0.25);
        L.Eval(L.Read("(clear-stepped-signals)"));
        REQUIRE(L.NumVal(L.Eval(L.Read("(sigval tri1)"))) == 1);
        L.Eval(L.Read("(clear-stepped-signals)"));
        REQUIRE(L.NumVal(L.Eval(L.Read("(sigval tri1)"))) == 0.25);
        L.Eval(L.Read("(clear-stepped-signals)"));
        REQUIRE(L.NumVal(L.Eval(L.Read("(sigval tri1)"))) == 0.5);
    }

    SECTION("sin-osc") {
        L.Eval(L.Read("(setq sin1 (sin-osc))"));
        REQUIRE(L.NumVal(L.Eval(L.Read("(get-slot sin1 'freq)"))) == 0);
        REQUIRE(L.NumVal(L.Eval(L.Read("(get-slot sin1 'out)"))) == 0);
        REQUIRE(procdraw::ApproximatelyEqual(L.NumVal(L.Eval(L.Read("(sigval sin1)"))), 0.5, 0.01));

        L.Eval(L.Read("(put-slot sin1 'freq (/ 4))"));
        L.Eval(L.Read("(clear-stepped-signals)"));
        REQUIRE(procdraw::ApproximatelyEqual(L.NumVal(L.Eval(L.Read("(sigval sin1)"))), 1, 0.01));
        L.Eval(L.Read("(clear-stepped-signals)"));
        REQUIRE(procdraw::ApproximatelyEqual(L.NumVal(L.Eval(L.Read("(sigval sin1)"))), 0.5, 0.01));
        L.Eval(L.Read("(clear-stepped-signals)"));
        REQUIRE(procdraw::ApproximatelyEqual(L.NumVal(L.Eval(L.Read("(sigval sin1)"))), 0, 0.01));
        L.Eval(L.Read("(clear-stepped-signals)"));
        REQUIRE(procdraw::ApproximatelyEqual(L.NumVal(L.Eval(L.Read("(sigval sin1)"))), 0.5, 0.01));
        L.Eval(L.Read("(clear-stepped-signals)"));
        REQUIRE(procdraw::ApproximatelyEqual(L.NumVal(L.Eval(L.Read("(sigval sin1)"))), 1, 0.01));
    }

    SECTION("Event Relay Signal") {
        L.Set(L.SymbolRef("relay1"), MakeEventRelaySignal(&L), L.Nil);
        REQUIRE(L.Null(L.Eval(L.Read("(sigval relay1)"))));
        L.Eval(L.Read("(put-slot relay1 'event true)"));
        REQUIRE(L.Null(L.Eval(L.Read("(sigval relay1)"))));
        L.Eval(L.Read("(clear-stepped-signals)"));
        REQUIRE(L.BoolVal(L.Eval(L.Read("(sigval relay1)"))));
        REQUIRE(L.BoolVal(L.Eval(L.Read("(sigval relay1)"))));
        L.Eval(L.Read("(clear-stepped-signals)"));
        REQUIRE(L.Null(L.Eval(L.Read("(sigval relay1)"))));
    }
}
