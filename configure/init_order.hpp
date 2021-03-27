#ifndef xpack_configure_init_order
#define xpack_configure_init_order
#include"macro/xexport.hpp"

namespace mixc::configure_init_order::origin{
    enum init_order_t{
        // 主线程 mutex 初始化
        the_concurrency_thread_self,

        // 控制台初始化模块
        the_io_private_tty  = 16,

        // 单元测试模块
        the_test,
    };
}

#endif

xexport_space(mixc::configure_init_order::origin)
