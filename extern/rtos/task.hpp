#ifndef xpack_extern_rtos_task
#define xpack_extern_rtos_task
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::extern_rtos_task::inc
#include"define/base_type.hpp"
#include"macro/xstruct.hpp"
#include"utils/bit_indicator.hpp"
#pragma pop_macro("xuser")

#define xtask_max_priority          32
#define xtask_time_slice            10'000 /*us*/
#define xtask_stack_guard_bytes     8
#define xtask                       ::mixc::extern_rtos_task::origin::task_sugar{} * [](voidp arg_ptr)

namespace mixc::extern_rtos_task::origin{
    struct task_context{
        task_context *  prev;
        task_context *  next;
        uxx             sp_mem;             // 栈指针内存起始地址
        uxx             sp_cur;             // 栈指针
        u16             stack_scale;        // 栈空间大小 32bytes pcs
        u08             priority;           // 优先级
        u08             priority_current;   // 当前优先级
    };

    struct task_list{
        using idc_t = inc::bit_indicator<xtask_max_priority>;
        using itm_t = task_context * [xtask_max_priority + 1/*one for defer*/];

        void push(task_context * task){
            uxx priority            = task->priority;

            // 当前链表无元素
            if (idc.get(priority) == false){
                idc.set(priority);
                list[priority]      = task;
                task->prev          = task;
                task->next          = task;
                return;
            }

            auto head               = list[priority];
            auto last               = head->prev;
            head->prev              = task;
            last->prev              = task;
            task->next              = head;
            task->prev              = last;
        }

        task_context * pop(){
            auto i                  = idc.index_of_first_set();

            // TODO：返回空闲任务
            if (i == not_exist){
                return nullptr;
            }

            auto head               = list[i];
            auto last               = head->prev;
            auto next               = head->next;

            if (head == next){
                idc.reset(i);
            }
            else{
                list[i]             = next;
                next->prev          = last;
                last->next          = next;
            }
            return head;
        }

        void push_defer(task_context * task){
            auto & last             = list[xtask_max_priority];
            task->next              = last;
            last                    = task;
        }

        void push_defer_emit(){
            auto & last             = list[xtask_max_priority];
            auto   next             = last;

            while(last != nullptr){
                next                = last->next;
                push(last);
                last                = next;
            }
        }

    private:
        itm_t   list {};
        idc_t   idc  {};
    };

    struct task_sugar{
        template<class lambda_t>
        auto operator * (lambda_t const &){
            struct closure{
                static void invoke(voidp arg_ptr){
                    lambda_t()(arg_ptr);
                }
            };
            return & closure::invoke;
        }
    };

    using invoke_t = void(*)(voidp);

    xstruct(
        xname(task_config),
        xprif(pstack_size,          uxx),
        xprif(ppriority,            u08),
        xprif(puse_stack_guard,     u08),
        xprif(pinvoke,              invoke_t),
        xprif(parg_ptr,             voidp)
    )
        using final = task_config;

        task_config():
            pstack_size(4096),
            ppriority(0),
            puse_stack_guard(false),
            pinvoke(& do_nothing),
            parg_ptr(nullptr){
        }

        xpubget_pubset(stack_size)
        xpubget_pubset(priority)
        xpubget_pubset(invoke)
        xpubget_pubset(arg_ptr)
        xpubget_pubset(use_stack_guard)
    private:
        static void do_nothing(voidp){
            ;
        }
    $

    using task_handler = uxx;

    extern void task_create(task_handler * handler, task_config const & tc);
    extern void task_run(task_handler handler);
    extern void task_exit(uxx exit_code = 0);
    extern void task_yield();
    extern void task_sleep(uxx ms);
}

#endif
