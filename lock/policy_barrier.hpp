// 注意：
// 在使用 when<>::can<> 模板时，参数可以不按顺序，但必须保证参数是从 0 到 n 每次步进为 1
#ifndef xpack_lock_policy_barrier
#define xpack_lock_policy_barrier
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::lock_policy_barrier
#include"lock/atom_and.hpp"
#include"lock/atom_fetch_or.hpp"
#include"lock/private/thread_yield.hpp"
#include"meta/fit_bits.hpp"
#include"meta_seq/tget.hpp"
#include"meta_seq/tlist.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::lock_policy_barrier{
    using namespace inc;

    enum{
        max_concurrency      = 4,
        max_concurrency_mask = (1 << max_concurrency) - 1,
    };

    template<class previous, class ... result>
    inline auto make_order(previous, tlist<result...>, tlist<>){
        return tlist<result...>();
    }

    template<class previous, class first, class ... rest, class ... result>
    inline auto make_order(previous, tlist<result...>, tlist<first, rest...>){
        if constexpr(previous::index + 1 == first::index){
            return make_order(first(), tlist<result..., first>(), tlist<rest...>());
        }
        else{
            return make_order(previous(), tlist<result...>(), tlist<rest..., first>());
        }
    }

    template<auto do_something, auto ... do_something_else>
    struct can_also{
        enum : uxx{
            also    = (... + (uxx(max_concurrency_mask) << (uxx(do_something_else) * max_concurrency))),
            master  = (uxx(max_concurrency_mask) << (uxx(do_something) * max_concurrency)),
            index   = (do_something),
        };
    };

    template<auto do_something>
    struct can_also<do_something>{
        enum : uxx{
            also    = (0),
            master  = (uxx(max_concurrency_mask) << (uxx(do_something) * max_concurrency)),
            index   = (do_something),
        };
    };

    template<auto do_something>
    struct when{
        template<auto ... do_something_else>
        using can = can_also<do_something, do_something_else...>;
    };

    struct dummy_previous{
        enum : uxx{
            index   = uxx(-1)
        };
    };

    template<class ... rules>
    using state_t = fit_bits<sizeof...(rules) * max_concurrency>;

    template<class ... rules>
    xstruct(
        xtmpl(policy_barrier, rules...),
        xprif(state, state_t<rules...>)
    )
        using rule_list = decltype(
            make_order(dummy_previous(), tlist<>(), tlist<rules...>())
        );

        using bits_t = state_t<rules...>;

        template<auto operation>
        uxx try_lock(){
            using rule  = typename tget<rule_list, operation>::item;
            uxx mutex   = ~rule::also;
            uxx mask    = uxx(1) << (uxx(operation) * max_concurrency);

            // 可以执行相同的操作
            if constexpr (rule::master & rule::also){
                for(uxx i = 0; i < max_concurrency; i++, mask <<= 1){
                    uxx old = atom_fetch_or<bits_t>(xref state, mask);

                    // 存在互斥操作
                    if ((old & mutex) != 0){
                        if ((old & mask) == 0){
                            atom_and<bits_t>(xref state, ~mask);
                        }
                        return not_exist;
                    }
                    // 当前位未被占用，则使用通道 0
                    if ((old & mask) == 0){
                        return i;
                    }
                }
                return not_exist;
            }
            // 相同的操作不能同时进行
            else{
                // 当前位被占用，无法操作
                uxx old = atom_fetch_or<bits_t>(xref state, mask);
                if (old & mask){
                    return not_exist;
                }

                // 后判断互斥操作，因为此处的 operation 可以与自己互斥
                if (old & mutex){
                    atom_and<bits_t>(xref state, ~mask);
                    return not_exist;
                }
                else{
                    return 0;
                }
            }
        }

        template<auto operation>
        uxx lock(){
            while(true){
                if (uxx channel = try_lock<operation>(); channel != not_exist){
                    return channel;
                }
                thread_yield();
            }
        }

        template<auto operation, class callback>
        void lock(callback const & call){
            uxx channel = lock<operation>();
            call();
            unlock<operation>(channel);
        }

        template<auto operation>
        void unlock(uxx channel){
            using rule  = typename tget<rule_list, operation>::item;
            uxx mask    = uxx(1) << (uxx(operation) * max_concurrency + channel);
            atom_and<bits_t>(xref state, ~mask);
        }
    $
}

namespace mixc::lock_policy_barrier::origin{
    using mixc::lock_policy_barrier::policy_barrier;
    using mixc::lock_policy_barrier::when;
}

#endif

namespace xuser::inc{
    using namespace ::mixc::lock_policy_barrier::origin;
}
