#ifndef xpack_configure_init_order
#define xpack_configure_init_order
#include"macro/xexport.hpp"

namespace mixc::configure_init_order::origin{
    enum init_order_t{
        the_io_private_tty  = 16,
    };
}

#endif

xexport_space(mixc::configure_init_order::origin)
