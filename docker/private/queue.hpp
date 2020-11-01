#ifndef xpack_docker_queue
#define xpack_docker_queue
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::docker_queue::inc
#include"dumb/disable_copy.hpp"
#include"dumb/struct_type.hpp"
#include"docker/transmitter.hpp"
#include"docker/private/single_linked_node.hpp"
#include"gc/self_management.hpp"
#include"interface/iterator.hpp"
#include"lock/atom_swap.hpp"
#include"lock/policy_barrier.hpp"
#include"memory/allocator.hpp"
#include"mixc.hpp"
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

    using with_queue_lock = policy_barrier<force_machine_word,
                        // routing, clear, push, pop, foreach, head_xr, head_xw
        when<routing>::can<                           foreach, head_xr        >,
        when<clear  >::can<>,
        when<push   >::can<                push              , head_xr, head_xw>::concurrency<co_max>,
        when<pop    >::can<>,
        when<foreach>::can<routing,                   foreach, head_xr, head_xw>::concurrency<co_max>,
        when<head_xr>::can<routing,        push,      foreach, head_xr, head_xw>::concurrency<co_max>,
        when<head_xw>::can<                push,      foreach, head_xr, head_xw>::concurrency<co_max>
    >;

    template<class final, class item_t, class barrier_t>
    xstruct(
        xtmpl(queue, final, item_t, barrier_t),
        xprib(node_field<item_t, barrier_t>),
        xpubb(self_management),
        xpubb(disable_copy),
        xasso(item_t)
    )
        using node      = single_linked_node<item_t>;
        using nodep     = node *;
        using base_t    = node_field<item_t, barrier_t>;
    protected:
        ~queue() {
            clear();
        }
    public:
        void clear() {
            nodep cur = nullptr;
            nodep tmp;
            nodep top;

            lock<opr::clear>([&](){
                top = base_t::swap_top(nullptr);
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
                free_with_destroy<node>(cur);
            } while (top != tmp);
        }

        void push(item_t const & value) {
            auto new_top = alloc_with_initial<node>(value);

            lock<opr::push>([&](){
                if (auto top = base_t::top(); top != nullptr){
                    new_top->next = top->next;
                    top->next     = new_top;
                    base_t::swap_top(new_top);
                }
                else{
                    new_top->next = new_top;
                    base_t::swap_top(new_top);
                }
            });
        }

        transmitter<item_t> pop() {
            transmitter<item_t> r;
            nodep               head = nullptr;

            lock<opr::pop>([&](){
                auto top = base_t::top(); 
                if (top == nullptr){
                    return;
                }

                head    = top->next;
                r       = *head;

                if (head == top){
                    base_t::swap_top(nullptr);
                }
                else{
                    top->next = head->next;
                }
            });

            if (head != nullptr){
                free_with_destroy<node>(head);
            }
            return r;
        }

        // 迭代器区
    private:
        template<auto mode, class iterator_t>
        void foreach_template(iterator_t const & invoke) const {
            lock<opr::foreach>([&](){
                nodep  top   = base_t::top();
                nodep  cur   = top;
                uxx    index = 0;
                loop_t state = loop_t::go_on;

                if (top == nullptr){
                    return;
                }

                do{
                    cur = cur->next;
                    xitr_switch(mode, index, state, invoke, *cur);
                }while(cur != top and state != loop_t::finish);
            });
        }
    public:
        xitr_foreach(item_t &)
        xitr_foreach_const(item_t const &)

        xpubget_pubsetx(head, transmitter<item_t>)
            xr{
                transmitter<item_t> r;
                lock<opr::head_xr>([&](){
                    if (nodep cur = base_t::top(); cur != nullptr){
                        r = cur->next[0];
                    }
                });
                return r;
            }
            xw{
                lock<opr::head_xw>([&](){
                    if (nodep cur = base_t::top(); cur != nullptr){
                        cur->next[0] = value;
                    }
                });
            }

        xpubgetx(is_empty, bool){
            return base_t::top() == nullptr;
        }
    $
}

#endif

#define xusing_docker_queue  ::mixc::docker_queue
