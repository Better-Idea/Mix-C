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
#include"dumb/struct_type.hpp"
#include"gc/self_management.hpp"
#include"interface/iterator.hpp"
#include"interface/ranger.hpp"
#include"lock/atom_swap.hpp"
#include"lock/policy_barrier.hpp"
#include"macro/xmflush.hpp"
#include"macro/xstruct.hpp"
#include"memory/allocator.hpp"
#pragma pop_macro("xuser")

namespace mixc::docker_stack{
    using namespace inc;

    enum opr{
        routing,
        clear,
        push,
        pop,
        foreach,

        // 下面属性不受约束
        // top
        // is_empty 
    };

    // push 时可以 foreach，但反过来时不行的，因为 push 只是局部锁住
    using barrier = policy_barrier<thread_counter_t,
                        // routing, clear, push, pop, foreach
        when<routing>::can<                push,      foreach>,
        when<clear  >::can<         clear, push              >,
        when<push   >::can<routing, clear, push              >,
        when<pop    >::can<>,
        when<foreach>::can<routing,        push,      foreach>
    >;

    template<class final>
    xstruct(
        xtmpl(barrier_component, final),
        xprob(barrier)
    ) $

    template<class final, class item_t>
    xstruct(
        xtmpl(stack_t, final, item_t),
        xprob(barrier_component<final>),
        xpubb(self_management),
        xpubb(disable_copy),
        xprof(ptop, mutable single_linked_node<item_t> *)
    )
    private:
        using node      = single_linked_node<item_t>;
        using nodep     = node *;
        using iteratorx = iteratorx<item_t &> const &;
        using iterator  = iterator <item_t &> const &;

        template<class guide>
        bool routing(){
            return false;    
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
            nodep cur = atom_swap(& the.ptop, nodep(nullptr));
            nodep tmp;

            barrier::lock_for<opr::clear>([&](){
                while(cur != nullptr){
                    tmp = cur;
                    cur = cur->next;
                    free_with_destroy<node>(tmp);
                }
            });
        }

        void push(item_t const & value) {
            nodep new_top  = alloc_with_initial<node>(value);
            new_top->next = nullptr;
            xmflush(); // 确保不和 clear 冲突

            barrier::lock_for<opr::push>([&](){
                new_top->next = atom_swap(& ptop, new_top);
            });
        }

        bool enter = false;

        transmitter<item_t> pop() {
            transmitter<item_t> r;
            nodep               free_item = nullptr;
            barrier::lock_for<opr::pop>([&](){
                if (enter){
                    xhint("error");
                }
                if (ptop == nullptr){
                    return;
                }
                enter = true;

                free_item   = ptop;
                r           = ptop[0];
                ptop        = ptop->next;

                enter = false;
            });

            if (free_item){
                free_with_destroy<node>(free_item);
            }
            return r;
        }

        // 迭代器区
    public:
        final & foreach(iteratorx itr){
            barrier::lock_for<opr::foreach>([&](){
                nodep cur = ptop;
                for(uxx i = 0; cur != nullptr; cur = cur->next, i++){
                    if (itr(i, cur[0]) == loop_t::finish){
                        return;
                    }
                }
            });
            return thex;
        }

        final & foreach(iterator itr){
            barrier::lock_for<opr::foreach>([&](){
                nodep cur = ptop;
                for(uxx i = 0; cur != nullptr; cur = cur->next, i++){
                    itr(i, cur[0]);
                }
            });
            return thex;
        }
        // 属性区
    public:
        xpubget_pubsetx(top, item_t &)
            xr{ return ptop[0]; }
            xw{ ptop[0] = value; }

        xpubgetx(is_empty, bool){
            return ptop == nullptr;
        }
    };

    template<class final, class item_t>
    using stack = adapter_pushpop<final, stack_t<final, item_t>, item_t>;
}

#endif

#define xusing_docker_stack     ::mixc::docker_stack
#include"math/index_system.hpp"

