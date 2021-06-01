#ifndef xpack_docker_queue
#define xpack_docker_queue
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::docker_queue::inc
#include"concurrency/lock/builtin_lock.hpp"
#include"concurrency/lock/policy_barrier.hpp"
#include"define/base_type.hpp"
#include"dumb/disable_copy.hpp"
#include"docker/transmitter.hpp"
#include"docker/private/single_linked_node.hpp"
#include"gc/self_management.hpp"
#include"macro/xexport.hpp"
#include"macro/xitr_foreach.hpp"
#include"macro/xstruct.hpp"
#include"meta/is_attached_lock.hpp"
#include"utils/allocator.hpp"
#pragma pop_macro("xuser")

namespace mixc::docker_queue {
    using namespace inc;

    enum opr{
        routing,
        clear,
        push,
        pop,
        foreach,
        head_xr,
        head_xw,
    };

    enum{
        co_max = (sizeof(uxx) * 8 - 4) / 4,
    };

    using with_queue_policy_lock = policy_barrier<force_machine_word,
                        // routing, clear, push, pop, foreach, head_xr, head_xw
        when<routing>::can<                           foreach, head_xr        >,
        when<clear  >::can<>,
        when<push   >::can<                push              , head_xr, head_xw>::concurrency<co_max>,
        when<pop    >::can<>,
        when<foreach>::can<routing,                   foreach, head_xr, head_xw>::concurrency<co_max>,
        when<head_xr>::can<routing,        push,      foreach, head_xr, head_xw>::concurrency<co_max>,
        when<head_xw>::can<                push,      foreach, head_xr, head_xw>::concurrency<co_max>
    >;

    template<class final_t, class item_t, is_attached_lock lock_t>
    xstruct(
        xtmpl(queue, final_t, item_t, lock_t),
        xpubb(self_management),
        xpubb(disable_copy),
        xprof(m_node, single_linked_node_ptr<item_t, lock_t>) // 带锁的节点指针类型
    )
    private:
        using node_t    = single_linked_node<item_t>;       // 纯节点类型
        using nodep     = node_t *;
    protected:
        ~queue() {
            clear();
        }
    public:
        void clear() {
            nodep cur = nullptr;
            nodep tmp;
            nodep top;

            m_node.template lock<opr::clear>([&](){
                top = m_node.swap_top(nullptr);
            });

            if (top == nullptr) {
                return;
            }
            else{
                tmp = top;
            }

            do {
                cur = tmp;
                tmp = tmp->next;
                free_with_destroy<node_t>(cur);
            } while (top != tmp);
        }

        void push(item_t const & value) {
            auto new_top = alloc_with_initial<node_t>(value);

            m_node.template lock<opr::push>([&](){
                if (auto top = m_node.top(); top != nullptr){
                    new_top->next = top->next;
                    top->next     = new_top;
                    m_node.swap_top(new_top);
                }
                else{
                    new_top->next = new_top;
                    m_node.swap_top(new_top);
                }
            });
        }

        transmitter<item_t> pop() {
            transmitter<item_t> r;
            nodep               head = nullptr;

            m_node.template lock<opr::pop>([&](){
                auto top = m_node.top(); 
                if (top == nullptr){
                    return;
                }

                head    = top->next;
                r       = *head;

                if (head == top){
                    m_node.swap_top(nullptr);
                }
                else{
                    top->next = head->next;
                }
            });

            if (head != nullptr){
                free_with_destroy<node_t>(head);
            }
            return r;
        }

        // 迭代器区
    private:
        template<auto mode_v, class iterator_t>
        void foreach_template(iterator_t const & invoke) const {
            m_node.template lock<opr::foreach>([&](){
                nodep  top      = m_node.top();
                nodep  cur      = top;
                uxx    index    = 0;

                if (top == nullptr){
                    return;
                }

                do{
                    cur         = cur->next;
                }while(inc::itr_switch<mode_v>(xref index, invoke, *cur) != loop_t::finish and cur != top);
            });
        }
    public:
        xitr_foreach(item_t)

        xpubget_pubsetx(head, transmitter<item_t>)
            xr{
                transmitter<item_t> r;
                m_node.template lock<opr::head_xr>([&](){
                    if (nodep cur = m_node.top(); cur != nullptr){
                        r = cur->next[0];
                    }
                });
                return r;
            }
            xw{
                m_node.template lock<opr::head_xw>([&](){
                    if (nodep cur = m_node.top(); cur != nullptr){
                        cur->next[0] = value;
                    }
                });
            }

        xpubgetx(is_empty, bool){
            return m_node.top() == nullptr;
        }
    $
}

#endif

#define xusing_docker_queue  ::mixc::docker_queue
