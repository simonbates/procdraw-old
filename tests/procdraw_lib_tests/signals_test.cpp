#include "signals.h"
#include "lisp_interpreter.h"
#include "catch.hpp"

TEST_CASE("Signals") {

    procdraw::LispInterpreter L;
    procdraw::RegisterSignals(&L);

    SECTION("step") {
        auto exp = R"(
            (setq s (make-signal (lambda (self)
              (put self 'val (+ (get self 'val) (get self 'incr))))))
        )";
        L.Eval(L.Read(exp));
        L.Eval(L.Read("(put s 'val 1)"));
        L.Eval(L.Read("(put s 'incr 2)"));
        REQUIRE(L.NumVal(L.Eval(L.Read("(get s 'val)"))) == 1);
        L.Eval(L.Read("('step s)"));
        REQUIRE(L.NumVal(L.Eval(L.Read("(get s 'val)"))) == 3);
        L.Eval(L.Read("('step s)"));
        REQUIRE(L.NumVal(L.Eval(L.Read("(get s 'val)"))) == 5);
    }

}
