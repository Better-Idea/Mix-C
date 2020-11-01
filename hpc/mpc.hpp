#ifndef xpack_phc_mpc
#define xpack_phc_mpc
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::phc_mpc::inc
#include"interface/can_callback.hpp"
#include"memory/allocator.hpp"
#include"lock/atom_fetch_or.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::phc_mpc{
    struct thread_info{
        u64         id;
        voidp       stack_top;
        u32         stack_bytes;
        u32         is_omp_stack    : 1;

        bool in_this_stack(voidp ptr){
            // TODO: 需要区分栈生长的方向================================
            auto stack_bottom = voidp(u08p(stack_top) - stack_bytes);
            return stack_bottom <= ptr and ptr <= stack_top;
        }
    };

    using core_invoke = inc::can_callback<loop_t(uxx, voidp)>;
    using core_lock_t = u64;

    struct loop_task{
        uxx         begin;
        uxx         end;
        core_invoke invoke;
    };
    
    void core_lock_init(voidp item);
    void core_lock_set(voidp item);
    void core_lock_unset(voidp item);
    void core_lock_deinit(voidp item);

    typedef struct core_control_block{
        core_lock_t     mutex;
    } ccb;

    struct mpc_t{
        static uxx  logic_cores();
        static uxx  current_core();
        static uxx  concurrency();
        static void concurrency(uxx value);
        static void break_point();
        static void yield();

        static void push(core_invoke const & call){
            
        }

        template<class main_invoke>
        static void init(main_invoke const & call){
            // TODO:enter once ====================
            // 设置最大并发量
            concurrency(
                logic_cores()
            );

            info_list = inc::alloc<ccb>(
                inc::memory_size{logic_cores() * sizeof(ccb)}
            );

            #pragma omp parallel
            {
                auto id = current_core();
                auto m  = xref info_list[id].mutex;
                core_lock_init(m);
                core_lock_set(m);

                if (id == 0){
                    call();
                }
                else{
                    // 上两次锁等待唤醒
                    inc::atom_fetch_or(xref free_indicator, uxx(1) << id);
                    core_lock_set(m);
                }
            }
        }
    private:
        ccb *   info_list;
        uxx     free_indicator = 0;
    };

    static inline mpc_t mpc;
}

#endif
