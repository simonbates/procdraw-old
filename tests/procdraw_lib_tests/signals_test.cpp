#include "signals.h"
#include "lisp_interpreter.h"
#include "catch.hpp"

TEST_CASE("Signals") {

    procdraw::LispInterpreter L;
    procdraw::RegisterSignals(&L);

    SECTION("step") {
        auto exp = R"(
            (setq s (make-signal (lambda (self)
              (put self (quote val) (+ (get self (quote val)) (get self (quote incr)))))))
        )";
        L.Eval(L.Read(exp));
        L.Eval(L.Read("(put s (quote val) 1)"));
        L.Eval(L.Read("(put s (quote incr) 2)"));
        REQUIRE(L.NumVal(L.Eval(L.Read("(get s (quote val))"))) == 1);
        L.Eval(L.Read("((quote step) s)"));
        REQUIRE(L.NumVal(L.Eval(L.Read("(get s (quote val))"))) == 3);
        L.Eval(L.Read("((quote step) s)"));
        REQUIRE(L.NumVal(L.Eval(L.Read("(get s (quote val))"))) == 5);
    }

}
