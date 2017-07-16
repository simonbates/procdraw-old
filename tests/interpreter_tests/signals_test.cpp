#include "procdraw/interpreter/signals.h"
#include "procdraw/interpreter/lisp_interpreter.h"
#include "procdraw/math.h"
#include <gtest/gtest.h>

class SignalsTest : public ::testing::Test {
protected:
    virtual void SetUp()
    {
        procdraw::RegisterSignals(&L_);
        L_.Eval(L_.Read("(setq step-incr (lambda (self) (put-slot self 'out (+ "
                        "(get-slot self 'out) (get-slot self 'incr)))))"));
        L_.Eval(L_.Read("(setq sig1 (make-signal step-incr))"));
        L_.Eval(L_.Read("(put-slot sig1 'out 1)"));
        L_.Eval(L_.Read("(put-slot sig1 'incr 2)"));
    }

    procdraw::LispInterpreter L_;
};

TEST_F(SignalsTest, GetSlot)
{
    EXPECT_EQ(1, L_.NumVal(L_.Eval(L_.Read("(get-slot sig1 'out)"))));
    EXPECT_EQ(2, L_.NumVal(L_.Eval(L_.Read("(get-slot sig1 'incr)"))));
}

TEST_F(SignalsTest, Sigval)
{
    EXPECT_EQ(3, L_.NumVal(L_.Eval(L_.Read("(sigval sig1)"))));
    EXPECT_EQ(3, L_.NumVal(L_.Eval(L_.Read("(sigval sig1)"))));
    L_.Eval(L_.Read("(clear-stepped-signals)"));
    EXPECT_EQ(5, L_.NumVal(L_.Eval(L_.Read("(sigval sig1)"))));
}

TEST_F(SignalsTest, SigvalWithNullStep)
{
    L_.Eval(L_.Read("(setq null-step-signal (make-signal nil))"));
    L_.Eval(L_.Read("(put-slot null-step-signal 'out 2)"));
    EXPECT_EQ(2, L_.NumVal(L_.Eval(L_.Read("(sigval null-step-signal)"))));
}

TEST_F(SignalsTest, ConnectWithSingleSigval)
{
    // sig1        sig2
    // incr  out   incr  out
    // 2     1     0     1     before
    // 2     3     3     4     step 1
    // 2     5     5     9     step 2
    L_.Eval(L_.Read("(setq sig2 (make-signal step-incr))"));
    L_.Eval(L_.Read("(put-slot sig2 'out 1)"));
    L_.Eval(L_.Read("(put-slot sig2 'incr 0)"));
    L_.Eval(L_.Read("(=> '(sigval sig1) sig2 'incr)"));
    EXPECT_EQ(4, L_.NumVal(L_.Eval(L_.Read("(sigval sig2)"))));
    EXPECT_EQ(4, L_.NumVal(L_.Eval(L_.Read("(sigval sig2)"))));
    EXPECT_EQ(3, L_.NumVal(L_.Eval(L_.Read("(sigval sig1)"))));
    EXPECT_EQ(3, L_.NumVal(L_.Eval(L_.Read("(sigval sig1)"))));
    L_.Eval(L_.Read("(clear-stepped-signals)"));
    EXPECT_EQ(9, L_.NumVal(L_.Eval(L_.Read("(sigval sig2)"))));
    EXPECT_EQ(5, L_.NumVal(L_.Eval(L_.Read("(sigval sig1)"))));
}

TEST_F(SignalsTest, ConnectWithExpressionOfSigval)
{
    // sig1        sig2
    // incr  out   incr  out
    // 2     1     0     1     before
    // 2     3     30    31    step 1
    // 2     5     50    81    step 2
    L_.Eval(L_.Read("(setq sig2 (make-signal step-incr))"));
    L_.Eval(L_.Read("(put-slot sig2 'out 1)"));
    L_.Eval(L_.Read("(put-slot sig2 'incr 0)"));
    L_.Eval(L_.Read("(=> '(* 10 (sigval sig1)) sig2 'incr)"));
    EXPECT_EQ(31, L_.NumVal(L_.Eval(L_.Read("(sigval sig2)"))));
    EXPECT_EQ(31, L_.NumVal(L_.Eval(L_.Read("(sigval sig2)"))));
    EXPECT_EQ(3, L_.NumVal(L_.Eval(L_.Read("(sigval sig1)"))));
    EXPECT_EQ(3, L_.NumVal(L_.Eval(L_.Read("(sigval sig1)"))));
    L_.Eval(L_.Read("(clear-stepped-signals)"));
    EXPECT_EQ(81, L_.NumVal(L_.Eval(L_.Read("(sigval sig2)"))));
    EXPECT_EQ(5, L_.NumVal(L_.Eval(L_.Read("(sigval sig1)"))));
}

TEST_F(SignalsTest, Saw)
{
    L_.Eval(L_.Read("(setq saw1 (saw))"));
    EXPECT_EQ(0, L_.NumVal(L_.Eval(L_.Read("(get-slot saw1 'freq)"))));
    EXPECT_EQ(0, L_.NumVal(L_.Eval(L_.Read("(get-slot saw1 'out)"))));
    EXPECT_EQ(0, L_.NumVal(L_.Eval(L_.Read("(sigval saw1)"))));

    L_.Eval(L_.Read("(put-slot saw1 'freq (/ 3 8))"));
    L_.Eval(L_.Read("(clear-stepped-signals)"));
    EXPECT_EQ(0.375, L_.NumVal(L_.Eval(L_.Read("(sigval saw1)"))));
    L_.Eval(L_.Read("(clear-stepped-signals)"));
    EXPECT_EQ(0.75, L_.NumVal(L_.Eval(L_.Read("(sigval saw1)"))));
    L_.Eval(L_.Read("(clear-stepped-signals)"));
    EXPECT_EQ(0.125, L_.NumVal(L_.Eval(L_.Read("(sigval saw1)"))));

    L_.Eval(L_.Read("(put-slot saw1 'freq (/ -5 8))"));
    L_.Eval(L_.Read("(clear-stepped-signals)"));
    EXPECT_EQ(0.5, L_.NumVal(L_.Eval(L_.Read("(sigval saw1)"))));
    L_.Eval(L_.Read("(clear-stepped-signals)"));
    EXPECT_EQ(0.875, L_.NumVal(L_.Eval(L_.Read("(sigval saw1)"))));
}

TEST_F(SignalsTest, Tri)
{
    L_.Eval(L_.Read("(setq tri1 (tri))"));
    EXPECT_EQ(0, L_.NumVal(L_.Eval(L_.Read("(get-slot tri1 'freq)"))));
    EXPECT_EQ(0, L_.NumVal(L_.Eval(L_.Read("(get-slot tri1 'out)"))));
    EXPECT_EQ(0, L_.NumVal(L_.Eval(L_.Read("(sigval tri1)"))));

    L_.Eval(L_.Read("(put-slot tri1 'freq (/ 4))"));
    L_.Eval(L_.Read("(clear-stepped-signals)"));
    EXPECT_EQ(0.5, L_.NumVal(L_.Eval(L_.Read("(sigval tri1)"))));
    L_.Eval(L_.Read("(clear-stepped-signals)"));
    EXPECT_EQ(1, L_.NumVal(L_.Eval(L_.Read("(sigval tri1)"))));
    L_.Eval(L_.Read("(clear-stepped-signals)"));
    EXPECT_EQ(0.5, L_.NumVal(L_.Eval(L_.Read("(sigval tri1)"))));
    L_.Eval(L_.Read("(clear-stepped-signals)"));
    EXPECT_EQ(0, L_.NumVal(L_.Eval(L_.Read("(sigval tri1)"))));
    L_.Eval(L_.Read("(clear-stepped-signals)"));
    EXPECT_EQ(0.5, L_.NumVal(L_.Eval(L_.Read("(sigval tri1)"))));

    L_.Eval(L_.Read("(put-slot tri1 'freq (/ -3 8))"));
    L_.Eval(L_.Read("(clear-stepped-signals)"));
    EXPECT_EQ(0.25, L_.NumVal(L_.Eval(L_.Read("(sigval tri1)"))));
    L_.Eval(L_.Read("(clear-stepped-signals)"));
    EXPECT_EQ(1, L_.NumVal(L_.Eval(L_.Read("(sigval tri1)"))));
    L_.Eval(L_.Read("(clear-stepped-signals)"));
    EXPECT_EQ(0.25, L_.NumVal(L_.Eval(L_.Read("(sigval tri1)"))));
    L_.Eval(L_.Read("(clear-stepped-signals)"));
    EXPECT_EQ(0.5, L_.NumVal(L_.Eval(L_.Read("(sigval tri1)"))));
}

TEST_F(SignalsTest, SinOsc)
{
    L_.Eval(L_.Read("(setq sin1 (sin-osc))"));
    EXPECT_EQ(0, L_.NumVal(L_.Eval(L_.Read("(get-slot sin1 'freq)"))));
    EXPECT_EQ(0, L_.NumVal(L_.Eval(L_.Read("(get-slot sin1 'out)"))));
    EXPECT_TRUE(procdraw::ApproximatelyEqual(
        L_.NumVal(L_.Eval(L_.Read("(sigval sin1)"))), 0.5, 0.01));

    L_.Eval(L_.Read("(put-slot sin1 'freq (/ 4))"));
    L_.Eval(L_.Read("(clear-stepped-signals)"));
    EXPECT_TRUE(procdraw::ApproximatelyEqual(
        L_.NumVal(L_.Eval(L_.Read("(sigval sin1)"))), 1, 0.01));
    L_.Eval(L_.Read("(clear-stepped-signals)"));
    EXPECT_TRUE(procdraw::ApproximatelyEqual(
        L_.NumVal(L_.Eval(L_.Read("(sigval sin1)"))), 0.5, 0.01));
    L_.Eval(L_.Read("(clear-stepped-signals)"));
    EXPECT_TRUE(procdraw::ApproximatelyEqual(
        L_.NumVal(L_.Eval(L_.Read("(sigval sin1)"))), 0, 0.01));
    L_.Eval(L_.Read("(clear-stepped-signals)"));
    EXPECT_TRUE(procdraw::ApproximatelyEqual(
        L_.NumVal(L_.Eval(L_.Read("(sigval sin1)"))), 0.5, 0.01));
    L_.Eval(L_.Read("(clear-stepped-signals)"));
    EXPECT_TRUE(procdraw::ApproximatelyEqual(
        L_.NumVal(L_.Eval(L_.Read("(sigval sin1)"))), 1, 0.01));
}

TEST_F(SignalsTest, EventRelaySignal)
{
    L_.Set(L_.SymbolRef("relay1"), MakeEventRelaySignal(&L_), L_.Nil);
    EXPECT_TRUE(L_.Null(L_.Eval(L_.Read("(sigval relay1)"))));
    L_.Eval(L_.Read("(put-slot relay1 'event true)"));
    EXPECT_TRUE(L_.Null(L_.Eval(L_.Read("(sigval relay1)"))));
    L_.Eval(L_.Read("(clear-stepped-signals)"));
    EXPECT_TRUE(L_.BoolVal(L_.Eval(L_.Read("(sigval relay1)"))));
    EXPECT_TRUE(L_.BoolVal(L_.Eval(L_.Read("(sigval relay1)"))));
    L_.Eval(L_.Read("(clear-stepped-signals)"));
    EXPECT_TRUE(L_.Null(L_.Eval(L_.Read("(sigval relay1)"))));
}

TEST_F(SignalsTest, ToggleSignal)
{
    L_.Eval(L_.Read("(setq toggle1 (toggle))"));
    EXPECT_EQ(0, L_.NumVal(L_.Eval(L_.Read("(sigval toggle1)"))));
    EXPECT_EQ(0, L_.NumVal(L_.Eval(L_.Read("(sigval toggle1)"))));
    L_.Eval(L_.Read("(clear-stepped-signals)"));
    EXPECT_EQ(0, L_.NumVal(L_.Eval(L_.Read("(sigval toggle1)"))));
    L_.Eval(L_.Read("(clear-stepped-signals)"));
    L_.Eval(L_.Read("(put-slot toggle1 'event true)"));
    EXPECT_EQ(1, L_.NumVal(L_.Eval(L_.Read("(sigval toggle1)"))));
    EXPECT_EQ(1, L_.NumVal(L_.Eval(L_.Read("(sigval toggle1)"))));
    L_.Eval(L_.Read("(clear-stepped-signals)"));
    L_.Eval(L_.Read("(put-slot toggle1 'event false)"));
    EXPECT_EQ(1, L_.NumVal(L_.Eval(L_.Read("(sigval toggle1)"))));
    L_.Eval(L_.Read("(clear-stepped-signals)"));
    L_.Eval(L_.Read("(put-slot toggle1 'event true)"));
    EXPECT_EQ(0, L_.NumVal(L_.Eval(L_.Read("(sigval toggle1)"))));
    L_.Eval(L_.Read("(clear-stepped-signals)"));
    EXPECT_EQ(1, L_.NumVal(L_.Eval(L_.Read("(sigval toggle1)"))));
}
