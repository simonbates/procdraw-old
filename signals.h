#pragma once

#include "lisp_interpreter.h"

namespace procdraw {

    LispObjectPtr MakeSignal(LispInterpreter *L, LispObjectPtr stepFun);
    LispObjectPtr GetSlot(LispInterpreter *L, LispObjectPtr signal, LispObjectPtr key);
    LispObjectPtr PutSlot(LispInterpreter *L, LispObjectPtr signal,
                          LispObjectPtr key, LispObjectPtr val);
    void RegisterSignals(LispInterpreter *L);

}
