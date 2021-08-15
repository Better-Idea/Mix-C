#ifndef xpack_configure_init_order
#define xpack_configure_init_order
#include"macro/xexport.hpp"

namespace mixc::configure_init_order::origin{
    enum init_order_t{
        // 主线程 mutex 初始化
        the_concurrency_thread_self,

        // 控制台初始化模块
        the_io_private_tty,

        // 内存回收单元
        the_mem,

        // GC 可回收对象筛选单元
        the_gc_filter,

        // GC 内存释放单元
        the_gc_release,

        // 单元测试模块
        the_test,

        // 主函数
        the_main,
        
        // 必须放在最后，表示初始化优先级个数
        the_end,
    };
}

#endif

xexport_space(mixc::configure_init_order::origin)
