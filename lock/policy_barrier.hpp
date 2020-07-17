// 注意：
// 在使用 when<>::can<> 模板时，参数可以不按顺序，但必须保证参数是从 0 到 n 每次步进为 1
#ifndef xpack_policy_barrier
#define xpack_policy_barrier
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::lock_policy_barrier
#include"lock/atom_and.hpp"
#include"lock/atom_fetch_add.hpp"
#include"lock/atom_fetch_and.hpp"
#include"lock/atom_fetch_or.hpp"
#include"lock/atom_sub.hpp"
#include"lock/atom_swap.hpp"
#include"lock/private/lock_state_t.hpp"
#include"meta/fit_bits.hpp"
#include"meta_seq/tget.hpp"
#include"meta_seq/tlist.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::lock_policy_barrier{
    using namespace inc;

    template<class previous, class ... result>
    inline auto make_order(previous, tlist<result...>, tlist<>){
        return tlist<result...>();
    }

    template<class previous, class first, class ... rest, class ... result>
    inline auto make_order(previous, tlist<result...>, tlist<first, rest...>){
        if constexpr(previous::master + 1 == first::master){
            return make_order(first(), tlist<result..., first>(), tlist<rest...>());
        }
        else{
            return make_order(previous(), tlist<result...>(), tlist<rest..., first>());
        }
    }

    template<auto do_something>
    struct when{
        template<auto ... do_something_else>
        struct can{
            static constexpr auto also             = (... + (uxx(1) << uxx(do_something_else)));
            static constexpr auto master           = (do_something);
        };
    };

    template<uxx number_of_counter, class count_t>
    struct meta{
        using bits_t = fit_bits<number_of_counter>;
        static constexpr bits_t no_clean = bits_t(-1);
        bits_t  bits    = 0;
        bits_t  clean   = no_clean;
        count_t count[number_of_counter] = {0};
    };

    struct dummy_previous{
        static constexpr uxx master = uxx(-1);
    };

    template<class count_t, class ... rules>
    struct policy_barrier : meta<sizeof...(rules), count_t>{
        using rule_list = decltype(
            make_order(dummy_previous(), tlist<>(), tlist<rules...>())
        );
        using base_t    = meta<sizeof...(rules), count_t>;

        template<auto operation>
        lock_state_t try_lock_for(){
            using rule  = typename tget<rule_list, operation>::item;
            uxx mask    = uxx(1) << operation;

            // 设置状态
            uxx old_bit;
            uxx old_cnt = atom_fetch_add(xref base_t::count[operation], 1);
            uxx clean   = atom_swap(xref base_t::clean, base_t::no_clean);

            // 谁先得到 clean，谁就更有机会不被 blocked
            // 这里为了对齐美观，做了一次无效的赋值
            old_bit     = atom_fetch_and(xref base_t::bits, clean);
            old_bit     = atom_fetch_or(xref base_t::bits, mask);

            // 假如不包含与当前操作互斥的操作，则可以进行该操作
            if ((old_bit & ~rule::also) == 0){
                return lock_state_t::accept;
            }
            else{
                unlock_for<operation>();
                return lock_state_t::blocked;
            }
        }

        template<auto operation>
        void unlock_for(){
            using rule = typename tget<rule_list, operation>::item;
            uxx mask   = uxx(1) << operation;

            if (atom_sub(xref base_t::count[operation], 1) == 0){
                atom_and(xref base_t::clean, ~mask);
            }

            // 错误的方式：
            // if (atom_sub(xref base_t::count[operation], 1) == 0){
            //     // 此夹缝导致即使其他线程给 bits 置位，也会被下面语句给复位
            //     // 所以我们干脆把复位的责任交给 try_lock_for
            //     atom_and(xref base_t::bits, ~mask);
            // }
        }
    };
}

namespace mixc::lock_policy_barrier::origin{
    using inc::lock_state_t;
    using mixc::lock_policy_barrier::policy_barrier;
    using mixc::lock_policy_barrier::when;
}

#endif

namespace xuser::inc{
    using namespace ::mixc::lock_policy_barrier::origin;
}
