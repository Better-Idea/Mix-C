#ifndef xpack_extern_rtos_task
#define xpack_extern_rtos_task
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::extern_rtos_task::inc
#include"define/base_type.hpp"
#include"extern/rtos/conf.hpp"
#include"macro/xcheck_binary_aligned.hpp"
#include"macro/xstruct.hpp"
#include"utils/bits_indicator.hpp"
#pragma pop_macro("xuser")

#define xtask                       ::mixc::extern_rtos_task::origin::task_sugar{} * [](voidp arg_ptr)

namespace mixc::extern_rtos_task::origin{
    typedef struct task_id_t{
        u64 number : 48;    // 每创建一个任务 number 就会加 1，该字段不会回收
        u64 id     : 16;    // 当有

        operator u64 (){
            return *u64p(this);
        }
    } tit_t;

    typedef struct task_context{
        using tcp_t = tcp_t;
        tcp_t   prev;
        tcp_t   next;
        tit_t   id;
        uxx     sp_base;            // 栈指针内存起始地址
        uxx     sp_current;         // 栈指针
        u16     stack_scale;        // 栈空间规模，需要乘以 unit_of_stack 才能得到实际栈大小
        u08     priority;           // 优先级
        u08     priority_current;   // 当前优先级
    } * tcp_t;

    struct task_list{
        using idc_t = inc::bits_indicator<inc::conf::max_priority>;
        using itm_t = tcp_t[inc::conf::max_priority + 1/*one for defer*/];

        void push(tcp_t task){
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

        tcp_t pop(){
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

        // 放弃一轮
        void push_defer(tcp_t task){
            auto & last             = this->defer_slot();
            task->next              = last;
            last                    = task;
        }

        void push_defer_emit(){
            auto & last             = this->defer_slot();
            auto   next             = last;

            while(last != nullptr){
                next                = last->next;
                this->push(last);
                last                = next;
            }
        }
    private:
        tcp_t & defer_slot(){
            return list[inc::conf::max_priority];
        }

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
        xprif(pstack_size,  uxx),
        xprif(parg_ptr,     voidp),
        xprif(pinvoke,      invoke_t),
        xprif(ppriority,    u08)
    )
        using final_t = task_config;

        task_config():
            pstack_size(inc::conf::default_scale_of_stack),
            ppriority(0),
            pinvoke(& do_nothing),
            parg_ptr(nullptr){
        }

        xpubget_pubset(stack_size)
        xpubget_pubset(priority)
        xpubget_pubset(invoke)
        xpubget_pubset(arg_ptr)
    private:
        static void do_nothing(voidp){
            ;
        }
    $

    struct task{
        task(){}
        task(task_config const & conf);

        void    run();
        void    yeild();
        void    suspend();
        void    suspend(uxx millisecond);
        void    resume();
        void    sleep(uxx millisecond);
        void    exit(uxx code = inc::conf::default_exit_code);
        tit_t   task_id();

        template<class lambda_t>
        void critical(lambda_t const & call){
            inc::conf::critical_enter();
            call();
            inc::conf::critical_exit();
        }

    private:
        tcp_t   handle = nullptr;
    };

    inline task this_task{};

    inline task::task(task_config const & conf){
        
    }

    inline void task::run(){

    }
}

#endif
