// 注意：
// 虽然 stack::push、stack::pop 等函数用了原子操作
// 但只保证了同一操作的原子性，如果想要同时使用 push/pop 需要上锁
#ifndef xpack_docker_stack
#define xpack_docker_stack
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::docker_stack
#include"define/base_type.hpp"
#include"docker/transmitter.hpp"
#include"docker/private/adapter.pushpop.hpp"
#include"docker/private/single_linked_node.hpp"
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
    template<class final, class item_t>
    xstruct(
        xtmpl(stack_t, final, item_t),
        xpubb(inc::self_management),
        xpubb(inc::disable_copy),
        xprof(ptop, mutable inc::single_linked_node<item_t> *)
    )
    private:
        using node      = inc::single_linked_node<item_t>;
        using nodep     = node *;
        using iteratorx = inc::iteratorx<item_t &> const &;
        using iterator  = inc::iterator <item_t &> const &;

        template<class guide>
        bool routing(){
            
        }

        // 构造析构区
    public:
        stack_t() : 
            ptop(nullptr){
        }
    protected:
        ~stack_t() {
            clear();
        }

        // 私有静态字段
    private:
        static inline nodep wait_free = nullptr;

        // 公有函数区
    public:
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
            nodep next = ptop->next;
            inc::free_with_destroy<node>(ptop);
            ptop = next;
            return r;
        }

        // 迭代器区
    public:
        final & foreach(iteratorx itr){
            inc::foreach(ptop, itr);
            return thex;
        }

        final & foreach(iterator itr){
            inc::foreach(ptop, [&](uxx index, item_t & value){
                itr(index, value);
                return loop_t::go_on;
            });
            return thex;
        }
        // 属性区
    public:
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
