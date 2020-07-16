#ifndef xpack_docker_queue
#define xpack_docker_queue
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::docker_queue
#include"define/base_type.hpp"
#include"dumb/disable_copy.hpp"
#include"dumb/struct_t.hpp"
#include"docker/transmitter.hpp"
#include"docker/private/adapter.foreach.hpp"
#include"docker/private/single_linked_node.hpp"
#include"gc/self_management.hpp"
#include"interface/ranger.hpp"
#include"lock/atom_swap.hpp"
#include"macro/xstruct.hpp"
#include"memory/allocator.hpp"
#pragma pop_macro("xuser")

namespace mixc::docker_queue {
    template<class item_t>
    xstruct(
        xtmpl(queue_t, item_t),
        xpubb(inc::self_management),
        xpubb(inc::disable_copy),
        xprif(pend, inc::single_linked_node<item_t>),
        xasso(item_t)
    )
        using node  = inc::single_linked_node<item_t>;
        using nodep = node *;
    public:
        queue_t():
            pend(nullptr) {
        }
    protected:
        ~queue_t() {
            clear();
        }
    public:
        void clear() {
            nodep cur  = inc::atom_swap(& the.pend, nodep(nullptr));
            nodep head = cur;
            nodep temp;

            if (cur == nullptr) {
                return;
            }

            do {
                temp = head;
                head = head->next;
                inc::free_with_destroy<node>(temp);
            } while (head != cur);
        }

        void push(item_t const & value) {
            auto new_end = inc::alloc_with_initial<node>(value);
            auto old_end = inc::atom_swap(& pend, new_end);

            if (old_end != nullptr) {
                new_end->next = old_end->next; // old_end->next 指向 head
                old_end->next = new_end;
            }
            else {
                new_end->next = new_end;
            }
        }

        inc::transmitter<item_t> pop() {
            auto head                  = pend->next;
            auto next                  = head->next;
            inc::transmitter<item_t> r = (item_t &)head[0];

            if (next != head) {
                pend->next = next;
            }
            else {
                pend = nullptr;
            }
            inc::free_with_destroy<node>(head);
            return r;
        }

        // 迭代器区
    public:
        final & foreach(iteratorx itr){
            if (pend != nullptr){
                inc::foreach(pend->next, itr);
            }
            return thex;
        }

        final & foreach(iterator itr){
            if (pend != nullptr){
                inc::foreach(pend->next, [&](uxx index, item_t & value){
                    itr(index, value);
                    return loop_t::go_on;
                });
            }
            return thex;
        }

        xpubgetx(head, item_t &){
            return pend->next[0];
        }

        xpubgetx(is_empty, bool){
            return pend == nullptr;
        }
    $

    template<class final, class item_t>
    using queue = inc::adapter_pushpop<final, queue_t<final, item_t>, item_t>;
}

#endif

#define xusing_docker_queue  ::mixc::docker_queue
