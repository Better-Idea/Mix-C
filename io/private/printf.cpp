#include<stdio.h>
#include"io/private/printf.hpp"

namespace mixc::io_private_printf{
    print_t const print_core = & printf;

    void print_flush(){
        fflush(stdout);
    }
}
