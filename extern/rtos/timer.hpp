#ifndef xpack_extern_rtos_timer
#define xpack_extern_rtos_timer
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::extern_rtos_timer::inc
#include"algo/heap_root.hpp"
#include"define/base_type.hpp"
#include"extern/rtos/conf.hpp"
#include"extern/rtos/memory.hpp"
#include"extern/rtos/task.hpp"
#include"macro/xexport.hpp"
#include"macro/xstruct.hpp"
#pragma pop_macro("xuser")

namespace mixc::extern_rtos_timer{
    enum{
        bits_of_is_short_task = 1,
        bits_of_tick          = 55,
    };
}

namespace mixc::extern_rtos_timer::origin{
    xstruct(
        xname(timer_config),
        xpubb(inc::task_config),
        xpric(m_is_short_task, bits_of_is_short_task, u64),
        xpric(m_tick, bits_of_tick, u64),
        xprif(m_i_position, uxx)
    )
        using final_t = the_t;

        timer_config():
            inc::task_config{},
            m_tick(0),
            m_i_position(0){
        }

        template<class>
        friend struct mixc::algo_heap_root::change_position;
        friend struct timer;

        xpubget_pubset(is_short_task)
        xpubget_pubset(tick)
        xpubget(i_position)
    $;

    using timer_configp = timer_config *;

    // 对外：
    // m_handler 虽然是指向 timer_config 的指针
    // 但应当避免将它当作 timer_config 直接访问
    xstruct(
        xname(timer_handler),
        xprif(m_tick, u64),
        xprif(m_handler, uxx)
    )
        timer_handler() : 
            m_tick{},
            m_handler{}{
        }

        friend struct timer;
    $;

    struct timer{
    private:
        static auto compare(){
            return []xcmp(timer_config *){
                union u_t{
                    struct{
                        u64  is_short_task : 1;
                        u64  priority      : 8;
                        u64  tick          : 55;
                    };
                    u64      value;
                };
                
                static_assert(
                    sizeof(inc::task_config::m_priority) * 8 + 
                    bits_of_is_short_task + 
                    bits_of_tick <= 64
                );

                u_t l, r;
                l.is_short_task     = left->is_short_task();
                r.is_short_task     = right->is_short_task();
                l.priority          = left->priority();
                r.priority          = right->priority();
                l.tick              = left->tick();
                r.tick              = right->tick();
                return l.value > r.value ? 1 : -1;
            };
        }
    public:
        timer():
            m_items{},
            m_count{},
            m_tick_current{}{
        }

        timer_handler create(timer_config const & item){
            timer_handler ret{};
            inc::g_task_self.critical([&](){
                auto mem            = inc::g_task_self.memory.alloc<timer_config>(item);
                ret.m_tick          = mem->tick() + m_tick_current;
                ret.m_handler       = uxx(mem);
                mem->tick(ret.m_tick);
                inc::heap_root::push(m_items, m_count, mem, timer::compare());
            });
            return ret;
        }

        void execute(){
            inc::g_task_self.critical([&](){
                for(u64 prev_tick = u64(-1); m_count != 0; ){
                    // 取出相同时间点的任务
                    // 首次取出 m_items 中的元素一定比 prev_tick 小
                    if (this->top_tick() > prev_tick){
                        return;
                    }
                    else{
                        prev_tick   = this->top_tick();
                    }

                    // m_count 需要放在 call(arg) 之前
                    // 因为 call 函数内部可能会调用 timer::create 新增元素
                    auto item       = inc::heap_root::pop(m_items, m_count, timer::compare());
                    m_count        -= 1;

                    // 短任务直接执行
                    // timer::execute 可能在中断函数中
                    // 所以短任务必须是那种可以迅速执行完毕，且占用资源较少的代码段
                    if (item->is_short_task()){
                        auto call   = item->callback();
                        auto arg    = item->arg_ptr();
                        call(arg, inc::conf::stdop_t::start);
                    }
                    // 其他的则需要创建任务单独执行
                    else{
                        inc::task{ *item };
                    }
                    inc::g_task_self.memory.free(item);
                }
            });
        }

        void increate_tick(uxx value){
            inc::g_task_self.critical([&](){
                m_tick_current     += value;
            });
        }

        uxx need_wait_tick(){
            uxx tick                = not_exist;
            inc::g_task_self.critical([&](){
                if (m_count == 0){
                    return;
                }

                if (m_tick_current <  this->top_tick()){
                    tick            = this->top_tick() - m_tick_current;
                }
                // 如果已经超时，则无需再等待
                else{
                    tick            = 0;
                }
            });
            return tick;
        }

        void cancel(timer_handler const & item){
            inc::g_task_self.critical([&](){
                // 如果堆是空的
                // 或者该定时任务已经不在堆中（tick 比堆顶小）
                if (m_count == 0 or item.m_tick < this->top_tick()){
                    return;
                }

                // 异常情况
                if (timer_configp(item.m_handler)->i_position() >= m_count){
                    return;
                }

                // 根据索引取出堆中的元素
                auto conf           = inc::heap_root::pop_by_index(
                    m_items, 
                    m_count, 
                    timer_configp(item.m_handler)->i_position(), 
                    timer::compare()
                );

                auto arg            = conf->arg_ptr();
                auto call           = conf->callback();
                call(arg, inc::conf::stdop_t::cancel);
                inc::g_task_self.memory.free(conf);
            });
        }

    private:
        u64 top_tick(){
            // 小根堆，m_items[0] 是最小的元素
            return m_items[0]->tick();
        }

        timer_config  * m_items[inc::conf::max_timer_count];
        uxx             m_count;
        u64             m_tick_current;
    };
}

namespace mixc::algo_heap_root{
    using namespace mixc::extern_rtos_timer::origin;

    template<> inline void change_position<timer_config *>::operator()(
        timer_config * const & item, uxx index
    ){
        auto & self         = (timer_config * &)item;
        self->m_i_position  = (index);
    }
}

#endif
