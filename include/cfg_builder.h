#pragma once

#include "cfg.h"

class CFGBuilder {
    public:
        CFG build(const TACProgram& program);
};

/*
ALGO

new block starts at:
- first instruction
- every label
- every instruction after a jmp

t0 = ...
ifFalse t0 goto L1

t1 = ...
goto L2

L1:

t2 = ...
L2:

return t2
*/

// becomes

/*
B0
----
to = ...
ifFalse ...

B1
----
t1 = ...
goto ...

B2
----
L1:
t2 = ...

B3
----
L2:
return ...
*/
