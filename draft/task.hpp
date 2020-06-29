#ifndef xpack_draft_task
#define xpack_draft_task
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::task
        #include<setjmp.h>
        #include"configure.hpp"
        #include"define/base_type.hpp"
        #include"docker/bit_indicator.hpp"
        #include"instruction/count_of_set.hpp"
        #include"memory/allocator.hpp"
    #pragma pop_macro("xuser")

    #define xmax_task_priority    32
    #ifndef xmax_task_priority
        #error "please define macro 'xmax_task_priority'"
    #endif

    namespace mixc::task::origin{
        using task_invoke_t = u32(*)(voidp);

        constexpr u16 lut_time_slice_us[] = {
            26000,
            24000,
            22000,
            20000,
            18000,
            16000,
            14000,
            12000,
            10000,
            8000,
            6000,
            4000,
            2500,
            1500,
            1000,
            500,
        };

        // 该结构存在对应任务的栈底
        struct task_control_block {
            enum {
                stack_scale_step          = 256, // 256B per step
                shift_for_priority_main   = 4,
                subpriority_count         = 1 << shift_for_priority_main,
                max_holdtimes             = 3,
            };

            void            *   memory;
            void            *   clean_stack;

            // 栈规模
            // 以 stack_scale_step 为单位
            u16                 stack_scale;

            // 当前任务在当前时间片下的响应表现
            // 置位位指示在规定的时间完成
            // 该字段为 fifo 结构
            // indicator_a 指示在当前优先级对应时间片下的表现
            // indicator_b 指示在高一档优先级对应对应时间片下的表现
            // indicator_a 当达标量低于阈值时，该任务的子优先级减一
            // indicator_b 当达标量高于阈值时，该任务的子优先级加一
            u16                 indicator_a;
            u16                 indicator_b;

            // 主优先级
            u16                 priority_main;

            // 子优先级
            u16                 priority_sub    : shift_for_priority_main;

            // 一个任务若连续执行超过三次，它的子优先级将会降到最低
            // 注：只要该任务
            u16                 holdtimes       : 2;

            uxx priority(){
                return priority_main << shift_for_priority_main | priority_sub;
            }

            void current_performance(uxx us){
                indicator_a <<= 1;
                indicator_a  |= us < lut_time_slice_us[priority_sub];

                if (priority_sub + 1 == subpriority_count){
                    return;
                }

                indicator_b <<= 1;
                indicator_b  |= us < lut_time_slice_us[priority_sub + 1];
            }

            void reevaluation_priority_sub(){
                constexpr uxx region = 10;

                if (inc::count_of_set(indicator_a) < region){
                    if (priority_sub != 0){
                        priority_sub -= 1;
                    }
                }
                else if (inc::count_of_set(indicator_b) >= region + 2){
                    if (priority_sub + 1 != subpriority_count){
                        priority_sub += 1;
                    }
                }
            }

            uxx timeslice(){
                return lut_time_slice_us[priority_sub];
            }
        };

        struct task_token{
            jmp_buf                 context;
            task_token          *   previous    = nullptr;
            task_token          *   next        = nullptr;
            task_control_block  *   tcb         = nullptr;

            task_token(task_control_block * tcb = nullptr) : 
                tcb(tcb){
            }
        } * ready_table[xmax_task_priority * task_control_block::subpriority_count];

        constexpr bool add_to_ready_table = true;

        task_control_block * task_current();
        void task_current(task_control_block * value);
        void task_environment_setup();
        void task_create(task_invoke_t invoke);
        void task_run();
        void task_stop();
        void task_wait();
        void task_exit();
        u64  task_us();
        void task_switch_disable();
        void task_switch_enable();
        void task_timeslice(uxx us);
        void task_switch(uxx cost_us, bool add_to_ready_table);


        inline static task_control_block                      info_main;
        inline static task_control_block *                    task_cur;
        inline static inc::bit_indicator<xmax_task_priority>  bmp_prio;

        task_control_block * task_current(){
            // TODO:
            return task_cur;
        }

        void task_current(task_control_block * value){
            // TODO:
            task_cur = value;
        }

        void task_environment_setup(){
            task_current(& info_main);
        }

        void task_create(task_invoke_t invoke){

            // uxx  stack_size = tcb.stack_scale * tcb.stack_scale_step;
            // u08p stack      = inc::alloc<u08>(inc::memory_size{stack_size});

            // 注意堆栈生长方向 ===========================================================
            typedef union buf_t{
                task_token      dat;
                _JUMP_BUFFER    regs;
            } * bufp;

            using tcbp = task_control_block *;
            auto ofs = inc::memory_size{1024};
            auto tcb = tcbp(inc::alloc<u08>(ofs)) + ofs;
            auto dat = uxxp(tcb - 1);
            auto buf = bufp(dat) - 1;

            if (setjmp(buf->dat.context) == 0){
                #if xis_x86
                    buf->regs.Rsp = buf->regs.Rbp = uxx(dat);
                #elif xis_arm
                    buf->regs.Sp = uxx(dat);
                #endif
            }
            else{
                invoke(nullptr);
                task_switch();
            }
        }

        void task_run(){

        }

        void task_stop(){

        }

        void task_wait(){

        }

        void task_exit(){

        }

        void task_switch_disable(){

        }

        void task_switch_enable(){

        }

        void task_timeslice(uxx us){

        }

        void task_switch(uxx cost_us, bool add_to_ready_table){
            // 得到当前最高优先级的任务
            auto i          = bmp_prio.index_of_last_set();
            auto cur        = task_current();
            auto token      = task_token(cur);

            // 根据本次执行耗时重新评估子优先级
            cur->current_performance(cost_us);
            cur->reevaluation_priority_sub();

            // 如果当前任务需要保存到就绪表
            if (add_to_ready_table){
                auto prio_main  = i >> (task_control_block::shift_for_priority_main);
                auto prio_sub   = i  & (task_control_block::subpriority_count - 1);

                if (prio_main < cur->priority_main or prio_main == not_exist){
                    task_switch_disable();
                    return;
                }
                if (prio_main == cur->priority_main and prio_sub < cur->priority_sub){
                    if (cur->holdtimes != cur->max_holdtimes){
                        cur->holdtimes++;
                        task_timeslice(cur->timeslice());
                        return;
                    }

                    cur->holdtimes    = 0;
                    cur->indicator_a  = ~0;
                    cur->indicator_b  = 0;
                    cur->priority_sub = 0;
                }

                auto prio       = cur->priority();
                auto ready      = ready_table[prio];
                auto next       = ready->next;
                auto last       = ready->previous;

                if (ready == nullptr){
                    token.next     =
                    token.previous =
                    ready_table[i] = & token;
                    bmp_prio.set(prio);
                }
                else{
                    last->next     =
                    next->previous = & token;
                    token.previous = last;
                    token.next     = next;
                }
            }

            auto ready      = ready_table[i];
            auto next       = ready->next;
            auto last       = ready->previous;

            if (next == ready){
                ready_table[i] = nullptr;
                bmp_prio.reset(i);
            }
            else{
                ready_table[i] = next;
                next->previous = last;
                last->next     = next;
            }

            ready           = ready_table[i];
            task_current(ready->tcb);
            task_timeslice(ready->tcb->timeslice());

            // token 会随着上下文的切换而被保存在栈上
            if (setjmp(token.context) == 0){
                longjmp(ready->context, 1);
            }
        }
    }
#endif

namespace xuser::inc{
    using namespace ::mixc::task::origin;
}
