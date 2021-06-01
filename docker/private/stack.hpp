#ifndef xpack_docker_stack
#define xpack_docker_stack
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::docker_stack::inc
#include"concurrency/lock/builtin_lock.hpp"
#include"concurrency/lock/policy_barrier.hpp"
#include"define/base_type.hpp"
#include"docker/transmitter.hpp"
#include"docker/private/single_linked_node.hpp"
#include"dumb/disable_copy.hpp"
#include"gc/self_management.hpp"
#include"macro/xexport.hpp"
#include"macro/xitr_foreach.hpp"
#include"macro/xstruct.hpp"
#include"meta/is_attached_lock.hpp"
#include"utils/allocator.hpp"
#pragma pop_macro("xuser")

namespace mixc::docker_stack{
    using namespace inc;

    enum opr{
        routing,
        clear,
        push,
        pop,
        foreach,
        top_xr,
        top_xw,
    };

    enum{
        co_max = (sizeof(uxx) * 8 - 4) / 4,
    };

    using with_stack_policy_lock = policy_barrier<force_machine_word,
                        // routing, clear, push, pop, foreach, top_xr, top_xw
        when<routing>::can<                           foreach, top_xr        >,
        when<clear  >::can<>,
        when<push   >::can<                push              , top_xr, top_xw>::concurrency<co_max>,
        when<pop    >::can<>,
        when<foreach>::can<routing,                   foreach, top_xr, top_xw>::concurrency<co_max>,
        when<top_xr >::can<routing,        push,      foreach, top_xr, top_xw>::concurrency<co_max>,
        when<top_xw >::can<                push,      foreach, top_xr, top_xw>::concurrency<co_max>
    >;

    template<class final_t, class item_t, is_attached_lock lock_t>
    xstruct(
        xtmpl(stack, final_t, item_t, lock_t),
        xpubb(self_management),
        xpubb(disable_copy),
        xprof(m_node, single_linked_node_ptr<item_t, lock_t>) // 带锁的节点指针类型
    )
    private:
        using node_t    = single_linked_node<item_t>;       // 纯节点类型
        using nodep     = node_t *;

        template<class guide>
        bool routing(){
            // TODO:==================================================
            return false;
        }

        // 构造析构区
    protected:
        ~stack() {
            clear();
        }

        // 公有函数区
    public:
        void clear() {
            nodep cur = nullptr;
            nodep tmp;

            m_node.template lock<opr::clear>([&](){
                cur = m_node.swap_top(nullptr);
            });

            while(cur != nullptr){
                tmp = cur;
                cur = cur->next;
                free_with_destroy<node_t>(tmp);
            }
        }

        void push(item_t const & value) {
            nodep new_top = alloc_with_initial<node_t>(value);
            m_node.template lock<opr::push>([&](){
                new_top->next = m_node.swap_top(new_top);
            });
        }

        transmitter<item_t> pop() {
            transmitter<item_t> r;
            nodep               free_item = nullptr;
            m_node.template lock<opr::pop>([&](){
                if (nodep cur = m_node.top(); cur != nullptr){
                    free_item = cur;
                    r         = cur[0];
                    m_node.swap_top(cur->next);
                }
            });

            if (free_item){
                free_with_destroy<node_t>(free_item);
            }
            return r;
        }

        // 迭代器区
    private:
        template<auto mode_v, class interator_t>
        void foreach_template(interator_t invoke) const {
            m_node.template lock<opr::foreach>([&](){
                nodep  cur   = m_node.top();
                uxx    index = 0;

                while(cur != nullptr){
                    if (inc::itr_switch<mode_v>(xref index, invoke, *cur) == loop_t::finish){
                        break;
                    }
                    cur = cur->next;
                }
            });
        }
    public:
        xitr_foreach(item_t)
        
        // 属性区
    public:
        xpubget_pubsetx(top, transmitter<item_t>)
            xr{
                transmitter<item_t> r;
                m_node.template lock<opr::top_xr>([&](){
                    if (nodep cur = m_node.top(); cur != nullptr){
                        r = cur[0];
                    }
                });
                return r;
            }
            xw{
                m_node.template lock<opr::top_xw>([&](){
                    if (nodep cur = m_node.top(); cur != nullptr){
                        cur[0] = value;
                    }
                });
            }

        // 注意：
        // 多线程环境即使读取到的 stack::is_empty() 为 false 也不能确定该 stack 非空
        xpubgetx(is_empty, bool){
            return m_node.top() == nullptr;
        }
    };
}

#endif

#define xusing_docker_stack     ::mixc::docker_stack
