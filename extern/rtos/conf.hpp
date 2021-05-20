#ifndef xpack_extern_rtos_conf
#define xpack_extern_rtos_conf
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::extern_rtos_conf::inc
#include"define/base_type.hpp"
#include"macro/xcheck_binary_aligned.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::extern_rtos_conf::origin{
    enum priority_of_task_t : u08{
        real_time = 0,

        // real_time task begin
        // =================================
        // TODO:
        // real_time_task0 = real_time,
        // real_time_task1,

        // =================================
        // real_time_task end

        normal = 16,

        // normal task begin
        // =================================
        // TODO:
        // normal_task0 = normal,
        // normal_task1,
        // ...

        // =================================
        // normal task end

        idlex = 30,

        // normal task begin
        // =================================
        // idldx 只有一个优先级，所以每个 idlex 任务的优先级必须相同
        // TODO:
        // idlex_task0 = idlex,
        // idlex_task1 = idlex,
        // ...

        // =================================
        // idlex task end
        idlex_check,

        idle = 31,

        // normal task begin
        // =================================
        // idle 只有一个优先级，所以每个 idle 任务的优先级必须相同
        // TODO:
        // idle_task0 = idle,
        // idle_task1 = idle,
        // ...

        // =================================
        // idle task end

        idle_check,
        max = idle_check,
    };

    constexpr uxx hz_of_task_tick_counter = 65536;
    constexpr uxx unit_of_stack = 32;           // 32byte 
    constexpr uxx max_task_count = 64;
    constexpr uxx default_scale_of_stack = 2048 / unit_of_stack;
    constexpr uxx default_priority = uxx(priority_of_task_t::normal);
    constexpr uxx default_exit_code = 0;
    constexpr uxx length_of_blocked_line = 8;
    constexpr uxx length_of_blocked_page = 
        (max_priority / length_of_blocked_line) + 
        (max_priority % length_of_blocked_line != 0);
    constexpr uxx max_priority = uxx(priority_of_task_t::max);

    xcheck_binary_aligned(hz_of_task_tick_counter);
    xcheck_binary_aligned(unit_of_stack);
    xcheck_binary_aligned(length_of_blocked_line);

    static_assert(length_of_blocked_line < max_priority * 2);
    static_assert(idlex + 1 == idlex_check, "idlex just has one priority");
    static_assert(idle  + 1 == idle_check , "idle just has one priority");

    enum class critical_enter_t{
        first_time,
        reentry,
    };

    enum class critical_exit_t{
        last_time,
        backtrack,
    };

    // 注意：!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // 非正常退出临界区需要复位该值
    // 对于不带高速缓存的设备适用
    // 多核设备可能不适用
    inline uxx task_lock_times = 0;

    inline critical_enter_t critical_enter(){
        // TODO：先关中断，保证只有一个任务进入
        // asm("close interrupt instruction");

        // ==========================
        return task_lock_times++ == 0 ? 
            critical_enter_t::first_time : 
            critical_enter_t::reentry;
    }

    inline critical_exit_t critical_exit(){
        if (--task_lock_times == 0){
            // TODO：开中断
            // asm("open interrupt instruction");
            return critical_exit_t::last_time;
        }
        return critical_exit_t::backtrack;
    }

    inline bool in_critical(){
        return task_lock_times != 0;
    }

    inline void switch_context(voidp next_task_stack_pointer){
        // TODO:
        // 将上下文保存到栈上
        // asm("save register");
        // asm("save register");
        // ...

        // 恢复
    }
};

#endif

xexport_spacex(conf, mixc::extern_rtos_conf::origin)
