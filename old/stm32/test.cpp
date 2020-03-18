#include"extern/stm32/firmware/core.hpp"

xseg(.entry) xmain()
    return 0;
$

#define inline 
xextc
    xdef(__aeabi_unwind_cpp_pr0) $
    xdef(__aeabi_unwind_cpp_pr1) $
    xdef(_start) $
$
#undef inline