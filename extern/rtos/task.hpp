#ifndef xpack_extern_rtos_task
#define xpack_extern_rtos_task
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::extern_rtos_task::inc
#include"define/base_type.hpp"
#include"extern/rtos/conf.hpp"
#include"extern/rtos/memory.hpp"
#include"macro/xcheck_binary_aligned.hpp"
#include"macro/xstruct.hpp"
#include"utils/bits_indicator.hpp"
#pragma pop_macro("xuser")

#define xtask                       ::mixc::extern_rtos_task::origin::task_sugar{} * [](voidp arg_ptr)

namespace mixc::extern_rtos_sync::origin{
    struct sync;
}

namespace mixc::extern_rtos_task::origin{
    using callback_t = void(*)(voidp);

    typedef struct task_id_t{
        u64 number : 48;    // 每创建一个任务 number 就会加 1，该字段不会回收
        u64 index  : 16;    // 当有

        operator u64(){
            return *u64p(this);
        }
    } tit_t;

    enum class task_state_t : u08{
        wait,       // 在等待信号
        suspend,    // 挂起
        ready,      // 就绪
        running,    // 运行
    };

    typedef struct task_context{
        using tcp_t = tcp_t;
        using clb_t = callback_t;
        using tst_t = task_state_t;
        tcp_t   prev;
        tcp_t   next;
        tit_t   id;
        voidp   sp_current;             // 栈指针
        voidp   arg_ptr;                // 回调函数参数
        clb_t   callback;               // 回调函数
        u16     stack_scale;            // 栈空间规模，需要乘以 unit_of_stack 才能得到实际栈大小
        u08     priority;               // 优先级
        u08     priority_current;       // 当前优先级
        tst_t   state : 4;              // 运行状态
    } * tcp_t;

    struct task_list{
        using idc_t = inc::bits_indicator<inc::conf::max_priority>;
        using itm_t = tcp_t[inc::conf::max_priority + 1/*one for defer*/];

        uxx priority_of_first_ready() const {
            return bmp.index_of_first_set();
        }

        void switch_to_first_ready(){
            auto item = pop_for(task_state_t::running);
            this->switch_context_at(item->sp_current);
        }

        void switch_context_at(voidp stack){
            inc::conf::switch_context(stack);
        }

        void push_for(tcp_t task, task_state_t state){
            uxx     priority        = task->priority_current;
            tcp_t & head            = slot[priority]; 
            tcp_t   prev            = head->prev; 
            tcp_t   next            = prev->next;

            // 注意：
            // - suspend 和 ready 任务都是存放在 task_list 里面
            // - 只有当前优先级对应的槽里面有 ready 任务时，idc 对应的位才会被置位
            // - 如果一个槽同时存了 suspend 和 ready 任务，首元优先存放 ready 任务
            //   首元的前一个元素是 suspend 任务，该 suspend 任务的下一个元素是当前槽位于最后的 ready 任务
            //   这样是为了保证同优先级 ready 任务 push 的顺序性
            if (task->state = state; bmp.get(priority)){
                // 若当前槽全是 ready 任务
                // 当 task 仍是 ready 任务，则插入到 head 前面
                // 否则在插入到 head 前面的同时，让 task->next 指向 prev （位于最后的 ready 任务）
                if (prev->state == task_state_t::ready){
                    prev->next      = task;
                    head->prev      = task;
                    task->prev      = prev;
                    task->next      = state == task_state_t::ready ? head : prev;
                }
                // prev is suspend task
                // 此时插入 ready 任务
                else if (state == task_state_t::ready){
                    auto last       = prev->next;
                    task->prev      = last;
                    task->next      = last->next;
                    last->next      = task;
                    prev->next      = task;         // 位于最后的 suspend 指向位于最后的 ready 任务
                }
                // prev is suspend task
                // 此时插入 suspend 任务
                else{
                    prev->next      = task;
                    head->prev      = task;
                    task->prev      = prev;
                    task->next      = prev->next;   // 位于最后的 suspend 指向位于最后的 ready 任务
                }
            }
            else{
                prev->next          = task;
                head->prev          = task;
                task->next          = head;
                task->prev          = prev;

                // 当前槽都是 suspend 任务
                // 此时 push 一个 ready 任务
                if (state == task_state_t::ready){
                    head            = task;
                    bmp.set(priority);
                }
            }
        }

        tcp_t pop_for(task_state_t state){
            auto i                  = bmp.index_of_first_set();

            // TODO：返回空闲任务==============
            if (i == not_exist){
                return nullptr;
            }

            auto head               = slot[i];
            auto prev               = head->prev;
            auto next               = head->next;

            if (head->state = state; head == next){
                bmp.reset(i);
                return head;
            }

            // 只有全部变成 suspend 任务时才可以设置 prev->next
            if (next->state == task_state_t::suspend){
                bmp.reset(i);
                prev->next          = next;
            }

            next->prev              = prev;
            slot[i]                 = next;
            return head;
        }

        void remove_for(tcp_t task, task_state_t state){
            uxx     priority= task->priority_current;
            tcp_t & head            = slot[];
            tcp_t   prev            = task->prev;
            tcp_t   next            = task->next;

            if (head == task){
                head                = next;
            }

            if (task->state == task_state_t::suspend){
                if (next->state == task_state_t::suspend){
                    prev->next      = next;
                    next->prev      = prev;
                }
                // next->state == task_state_t::ready
                // 说明 task 当前槽位于最后的 suspend 任务， next 指向的也是位于最后的 ready 任务
                else if (prev->state == task_state_t::suspend){
                    prev->next      = next;
                    head->prev      = prev;
                }
                // prev->state == task_state_t::ready
                // task 是仅存当前槽的 suspend 任务
                else{
                    prev->next      = head;
                    head->prev      = prev;
                }
            }
            // task->state == task_state_t::ready
            else{
                if (next->prev = prev; 
                    prev->state == task_state_t::ready){
                    prev->next      = next;
                }
                // prev->state == task_state_t::suspend
                else if (next->state == task_state_t::suspend){
                    prev->next      = next;
                    bmp.reset(task->priority_current);
                }
                // prev->state == task_state_t::suspend
                // next->state == task_state_t::ready
                // pass
                else{
                    ;
                }
            }

            task->state             = state;
        }

        // 放弃一轮
        void push_defer_for(tcp_t task, task_state_t state){
            auto & last             = this->defer_slot();
            task->next              = last;
            task->state             = state;
            last                    = task;
        }

        void push_defer_emit(){
            auto & last             = this->defer_slot();
            auto   next             = last;

            while(last != nullptr){
                next                = last->next;
                this->push_for(last, last->state);
                last                = next;
            }
        }
    private:

        tcp_t & defer_slot(){
            return slot[inc::conf::max_priority];
        }

        itm_t   slot {};
        idc_t   bmp  {};
    };

    struct task_sugar{
        template<class lambda_t>
        auto operator * (lambda_t const &){
            struct closure{
                static void callback(voidp arg_ptr){
                    lambda_t()(arg_ptr);
                }
            };
            return & closure::callback;
        }
    };

    xstruct(
        xname(task_config),
        xprif(pstack_scale, uxx),
        xprif(parg_ptr,     voidp),
        xprif(pcallback,    callback_t),
        xprif(ppriority,    u08)
    )
        using final_t = task_config;

        task_config():
            pstack_scale(inc::conf::default_scale_of_stack),
            ppriority(inc::conf::default_priority),
            pcallback(& do_nothing),
            parg_ptr(nullptr){
        }

        xpubget_pubset(stack_scale)
        xpubget_pubset(priority)
        xpubget_pubset(callback)
        xpubget_pubset(arg_ptr)
    private:
        static void do_nothing(voidp){
            ;
        }
    $

    struct task{
        task() : memory(g_memory){}
        task(task_config const & conf);

        /**
         * @brief 让出时间片，允许其他任务执行，即使优先级比当前低
         */
        void yeild();

        /**
         * @brief 挂起任务，等待其他任务将此任务唤醒
         */
        void suspend();

        /**
         * @brief 挂起任务，等待其他任务将此任务唤醒或等到超时时间后自动唤醒
         * @param millisecond 超时的毫秒数
         */
        void suspend(uxx millisecond);

        /**
         * @brief 唤醒任务，该函数只有其他任务调用当前任务才有效
         */
        void resume();

        /**
         * @brief 退出任务
         * @param code 
         */
        void exit(uxx code = inc::conf::default_exit_code);

        /**
         * @brief 获取任务标识
         */
        tit_t id(){
            return m_handle->id;
        }

        /**
         * @brief 获取任务当前优先级
         */
        uxx priority_current(){
            return m_handle->priority_current;
        }

        /**
         * @brief 进入临界区，避免被打断
         * @tparam lambda_t 回调函数类型，符合 void(*)() 格式
         * @param call 回调函数
         */
        template<class lambda_t>
        void critical(lambda_t const & call){
            inc::conf::critical_enter();
            call();
            inc::conf::critical_exit();
        }

        friend bool operator == (task const & self, decltype(nullptr)){
            return self.m_handle == nullptr;
        }

        friend bool operator == (decltype(nullptr), task const & self){
            return self.m_handle == nullptr;
        }

        friend bool operator != (task const & self, decltype(nullptr)){
            return self.m_handle != nullptr;
        }

        friend bool operator != (decltype(nullptr), task const & self){
            return self.m_handle != nullptr;
        }
    public:
        inc::memory & memory;
    private:
        tcp_t   m_handle = nullptr;

        friend mixc::extern_rtos_sync::origin::sync;
    };

    inline task         g_task_self{};
    inline task_list    g_task_list{};
    inline inc::memory  g_memory = {};
    inline u64          g_task_number = 0;

    inline task::task(task_config const & conf) :
        memory(g_memory){

        this->critical([&](){
            auto stack_bytes            = conf.stack_scale() * inc::conf::unit_of_stack;
            auto total_bytes            = stack_bytes + sizeof(task_context);
            auto sp_base                = memory.alloc(stack_bytes);

            if (m_handle = tcp_t(memory.alloc(total_bytes)); m_handle == nullptr){
                return; // error log
            }

            if (g_task_self == nullptr){
                g_task_self.m_handle    = m_handle;
                m_handle->prev          = m_handle;
                m_handle->next          = m_handle;
                m_handle->state         = task_state_t::running;
            }
            else{
                g_task_list.push_for(m_handle, task_state_t::ready);
            }

            m_handle->id.number         = g_task_number++;
            m_handle->id.index          = 0;        // TODO:=============
            m_handle->sp_current        = nullptr;  // TODO:=============
            m_handle->arg_ptr           = conf.arg_ptr();
            m_handle->callback          = conf.callback();
            m_handle->priority          = conf.priority();
            m_handle->priority_current  = conf.priority();
        });
    }

    inline void task::yeild(){
        if (auto next_task = tcp_t{};
            g_task_self.m_handle == m_handle and
            m_handle->state == task_state_t::running){

            this->critical([&](){
                g_task_list.push_defer_emit();
                next_task               = g_task_list.pop_for(task_state_t::running); 
                g_task_list.push_defer_for(m_handle, task_state_t::ready);
            });

            g_task_list.switch_context_at(next_task->sp_current);
        }
    }

    inline void task::suspend(){
        // TODO：支持被动挂起========================
        if (auto next_task = tcp_t{};
            g_task_self.m_handle == m_handle and
            m_handle->state == task_state_t::running){

            this->critical([&](){
                g_task_list.push_defer_emit();
                next_task                   = g_task_list.pop_for(task_state_t::running);
                g_task_list.push_for(m_handle, task_state_t::suspend);
            });

            g_task_list.switch_context_at(next_task->sp_current);
        }
    }

    inline void task::suspend(uxx millisecond){
        // TODO：定时器=============================
    }

    inline void task::resume(){
        bool can_runing_now                 = false;

        // TODO：考虑 m_handle->state == task_state_t::wait 等待其他信号的情况
        this->critical([&](){
            if (m_handle->state != task_state_t::suspend){
                return;
            }
            // 从列表中移除当前 suspend 任务，并设置为 runing 态
            else{
                g_task_list.remove_for(m_handle, task_state_t::running);
            }

            // resume 的任务优先级比当前低
            // 以 ready 态存回列表
            if (m_handle->priority_current >= g_task_self.m_handle->priority_current){
                g_task_list.push_for(m_handle, task_state_t::ready);
            }
            else{
                can_runing_now              = true;
            }
        });

        if (can_runing_now){
            g_task_list.switch_context_at(m_handle->sp_current);
        }
    }

    inline void task::exit(uxx code){
        // TODO：===================================
    }
}

#endif

xexport_space(mixc::extern_rtos_task::origin)
