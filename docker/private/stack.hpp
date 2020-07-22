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
        top_xr,
        top_xw,
    };

    enum{
        co_max = (sizeof(uxx) * 8 - 4) / 4,
    };

    using with_stack_lock = policy_barrier<force_machine_word,
                        // routing, clear, push, pop, foreach, top_xr, top_xw
        when<routing>::can<                           foreach, top_xr        >,
        when<clear  >::can<>,
        when<push   >::can<                push              , top_xr, top_xw>::concurrency<co_max>,
        when<pop    >::can<>,
        when<foreach>::can<routing,                   foreach, top_xr, top_xw>::concurrency<co_max>,
        when<top_xr >::can<routing,        push,      foreach, top_xr, top_xw>::concurrency<co_max>,
        when<top_xw >::can<                push,      foreach, top_xr, top_xw>::concurrency<co_max>
    >;

    template<class final, class item_t, class lock_t>
    xstruct(
        xtmpl(stack_t, final, item_t, lock_t),
        xprob(node_field<item_t, lock_t>),
        xpubb(self_management),
        xpubb(disable_copy)
    )
    private:
        using base_t    = node_field<item_t, lock_t>;
        using node      = single_linked_node<item_t>;
        using nodep     = node *;
        using iteratorx = iteratorx<item_t &> const &;
        using iterator  = iterator <item_t &> const &;

        template<class guide>
        bool routing(){
            // TODO:==================================================
            return false;
        }

        // 构造析构区
    public:
        stack_t(){
            base_t::ptop = nullptr;
        }
    protected:
        ~stack_t() {
            clear();
        }

        // 公有函数区
    public:
        void clear() {
            nodep cur = nullptr;
            nodep tmp;

            lock<opr::clear>([&](){
                cur = base_t::swap_top(nullptr);
            });

            while(cur != nullptr){
                tmp = cur;
                cur = cur->next;
                free_with_destroy<node>(tmp);
            }
        }

        void push(item_t const & value) {
            nodep new_top = alloc_with_initial<node>(value);
            lock<opr::push>([&](){
                new_top->next = base_t::swap_top(new_top);
            });
        }

        transmitter<item_t> pop() {
            transmitter<item_t> r;
            nodep               free_item = nullptr;
            lock<opr::pop>([&](){
                if (nodep cur = base_t::top(); cur != nullptr){
                    free_item = cur;
                    r         = cur[0];
                    base_t::swap_top(cur->next);
                }
            });

            if (free_item){
                free_with_destroy<node>(free_item);
            }
            return r;
        }

        // 迭代器区
    public:
        final & foreach(iteratorx itr){
            lock<opr::foreach>([&](){
                nodep cur = base_t::top();
                for(uxx i = 0; cur != nullptr; cur = cur->next, i++){
                    if (itr(i, cur[0]) == loop_t::finish){
                        return;
                    }
                }
            });
            return thex;
        }

        final & foreach(iterator itr){
            lock<opr::foreach>([&](){
                nodep cur = base_t::top();
                for(uxx i = 0; cur != nullptr; cur = cur->next, i++){
                    itr(i, cur[0]);
                }
            });
            return thex;
        }
        // 属性区
    public:
        xpubget_pubsetx(top, transmitter<item_t>)
            xr{
                transmitter<item_t> r;
                lock<opr::top_xr>([&](){
                    if (nodep cur = base_t::top(); cur != nullptr){
                        r = cur[0];
                    }
                });
                return r;
            }
            xw{
                lock<opr::top_xw>([&](){
                    if (nodep cur = base_t::top(); cur != nullptr){
                        cur[0] = value;
                    }
                });
            }

        // 注意：
        // 多线程环境即使读取到的 stack::is_empty() 为 false 也不能确定该 stack 非空
        xpubgetx(is_empty, bool){
            return base_t::top() == nullptr;
        }
    };

    template<class final, class item_t, class lock>
    using stack = adapter_pushpop<final, stack_t<final, item_t, lock>, item_t>;
}

#endif

#define xusing_docker_stack     ::mixc::docker_stack
#include"math/index_system.hpp"

