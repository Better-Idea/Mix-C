#ifndef xpack_docker_stack
#define xpack_docker_stack
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::docker_stack
#include"define/base_type.hpp"
#include"docker/transmitter.hpp"
#include"docker/private/adapter.pushpop.hpp"
#include"dumb/disable_copy.hpp"
#include"dumb/struct_t.hpp"
#include"gc/self_management.hpp"
#include"interface/iterator.hpp"
#include"interface/ranger.hpp"
#include"lock/atom_swap.hpp"
#include"macro/xstruct.hpp"
#include"memory/allocator.hpp"
#pragma pop_macro("xuser")

namespace mixc::docker_stack{
    template<class item_t>
    xstruct(
        xtmpl(node_t, item_t),
        xpubb(inc::struct_t<item_t>),
        xpubf(next, node_t<item_t> *)
    )
        template<class ... args>
        node_t(args const & ... list) :
            inc::struct_t<item_t>(list...), next(nullptr){
        }
    $

    template<class final, class item_t>
    xstruct(
        xtmpl(stack_t, final, item_t),
        xpubb(inc::self_management),
        xpubb(inc::disable_copy),
        xprof(ptop, mutable node_t<item_t> *)
    )
        using node      = node_t<item_t>;
        using nodep     = node *;
        using iteratorx = inc::iteratorx<item_t &> const &;
        using iterator  = inc::iterator <item_t &> const &;
    public:
        stack_t() : 
            ptop(nullptr){
        }
    protected:
        ~stack_t() {
            clear();
        }
    public:
        struct itr{
            itr(nodep top = nullptr) : 
                ptop(top){
            }
            item_t * operator -> (){
                return & operator item_t & ();
            }
            item_t const * operator -> () const {
                return & operator item_t & ();
            }
            operator item_t & (){
                return ptop[0];
            }
            operator item_t const & () const {
                return ptop[0];
            }
            bool finished() const {
                return ptop == nullptr;
            }
            itr next() const {
                return itr(ptop->next);
            }
        private:
            nodep ptop;
        };

        void clear() {
            nodep cur = inc::atom_swap(& the.ptop, nodep(nullptr));
            nodep tmp;
            while(cur != nullptr){
                tmp = cur;
                cur = cur->next;
                inc::free_with_destroy<node>(tmp);
            }
        }

        void push(item_t const & value) {
            auto new_top  = inc::alloc_with_initial<node>(value);
            new_top->next = inc::atom_swap(& ptop, new_top);
        }

        inc::transmitter<item_t> pop() {
            inc::transmitter<item_t> r = ptop[0];
            inc::free_with_destroy<node>(ptop);
            ptop = ptop->next;
            return r;
        }

        final & foreach(iteratorx itr){
            nodep cur   = the.ptop;
            uxx   index = 0;
            while(cur != nullptr){
                if (itr(index, cur[0]) == loop_t::finish){
                    break;
                }
                cur     = cur->next;
                index  += 1;
            }
            return thex;
        }

        final & foreach(iterator itr){
            return foreach([&](uxx index, item_t & value){
                itr(index, value);
                return loop_t::go_on;
            });
        }

        xpubget_pubset(top, &)

        xpubgetx(is_empty, bool){
            return ptop == nullptr;
        }
    };

    template<class final, class item_t>
    using stack = inc::adapter_pushpop<final, stack_t<final, item_t>, item_t>;
}

#endif

#define xusing_docker_stack     ::mixc::docker_stack
#include"math/index_system.hpp"

/*
        \   when
    can  \  top     pop     push    clear   routing
    top     true    false   true    false   true
    pop     false   false   false   false   false
    push    true    false   true    false   false
    clear   false   false   false   true    false
    routing true    false   false   false   false
*/
