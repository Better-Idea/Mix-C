// 注意：
// - 在使用 when<>::can<> 模板时，when 语句可以不按顺序，但必须保证连续性
//   可以是 do_sth0, do_sth1 或 do_sth1, do_sth2 这样组合，但不能是 do_sth0, do_sth2 这样中间缺少的组合
//   enum opr{
//      do_sth0,
//      do_sth1,
//      do_sth2,
//      ...
//   };
//   policy_barrier<
//                      // 这里推荐用表格的格式填参数
//                      // do_sth0, do_sth1, do_sth2
//      when<do_sth1>::can<         do_sth1>,
//      when<do_sth0>::can<do_sth0,          do_sth2>, 
//   >
// - 下文的同步区代指使用 policy_barrier 进行同步协调的函数
// - 每一个 when 语句至少需要消耗 1bit 指示位，请保证消耗的指示位总数不超过机器字长 uxx 的总位数
// 
// 优化：
// - 如果情况允许，policy_barrier::bits_t 和 uxx 类型（机器字长）保持一致可以获取完整的性能
// - 如果同步区耗时不足以忽略同步带来的开销，那么即使该同步区可以并发，也请做如下设置：
//   when<do_sth>::can<do_sth, do_sth_else...>::concurrency<1>
// - 对于一个耗时较长且可以并发的同步区，可以给一个较大的并发值，推荐给一个大于 cpu 核心数的值
#ifndef xpack_lock_policy_barrier
#define xpack_lock_policy_barrier
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::lock_policy_barrier
#include"lock/atom_and.hpp"
#include"lock/atom_fetch_or.hpp"
#include"lock/private/thread_yield.hpp"
#include"instruction/index_of_first_set.hpp"
#include"meta/fit_bits.hpp"
#include"meta/is_same.hpp"
#include"meta_seq/tget.hpp"
#include"meta_seq/tlist.hpp"
#include"meta_seq/vin.hpp"
#include"meta_seq/vlist.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::lock_policy_barrier{
    using namespace inc;

    template<class raw_data_list, class share_list>
    constexpr uxx share_mask = 0;

    template<class raw_data_list>
    constexpr uxx share_mask<raw_data_list, vlist<>> = 0;

    template<class raw_data_list, auto ... index>
    constexpr uxx share_mask<raw_data_list, vlist<index...>> = 
        (... | tget<raw_data_list, index>::mask);

    template<uxx offset, uxx max_con, class share_list>
    struct raw_data{
        enum : uxx{ 
            master          = (uxx(1) << (offset)),
            mask            = max_con == sizeof(uxx) * 8 ? uxx(-1) : ((uxx(1) << max_con) - 1) << offset,
            boundary        = offset + max_con,
            can_concurrency = max_con > 1,
        };
        using share_for = share_list;
    };

    template<uxx total_bits, class tlist>
    struct pair{
        using bits_t    = fit_bits<total_bits>;
        using new_list  = tlist;
    };

    struct utils{
    private:
        struct dummy{
            enum : uxx{ master = uxx(-1) };
        };

        template<class previous, class ... result>
        static auto make_order(previous, tlist<result...>, tlist<>){
            return tlist<result...>();
        }

        template<class previous, class first, class ... rest, class ... result>
        static auto make_order(previous, tlist<result...>, tlist<first, rest...>){
            if constexpr(previous::master + 1 == first::master){
                return make_order(first(), tlist<result..., first>(), tlist<rest...>());
            }
            else{
                return make_order(previous(), tlist<result...>(), tlist<rest..., first>());
            }
        }

        template<uxx offset, class first, class ... rest, class ... ready>
        static auto tidy(tlist<ready...>, tlist<first, rest...>){
            using new_item = raw_data<offset, first::max_concurrency, typename first::share_for>;
            using new_list = tlist<ready..., new_item>;
            enum : uxx{ new_offset = offset + first::max_concurrency };
            static_assert(new_offset <= sizeof(uxx) * 8);

            if constexpr (sizeof...(rest) == 0){
                return pair<new_offset, new_list>();
            }
            else{
                return tidy<new_offset>(new_list(), tlist<rest...>());
            }
        }
    public:
        template<class ... rules>
        static auto/*pair*/ build(){
            return tidy<0>(
                tlist<>(),
                make_order(dummy(), tlist<>(), tlist<rules...>())
            );
        }
    };

    template<auto max_con, auto do_sth, auto ... do_sth_else>
    struct flow{
    protected:
        friend struct utils;

        using share_for     = vlist<do_sth_else...>;
        enum{ 
            master          = do_sth, 
            max_concurrency = max_con,
        };
    };

    template<bool can_con, auto do_sth, auto ... do_sth_else>
    struct can_also;

    template<auto do_sth, auto ... do_sth_else>
    struct can_also<false, do_sth, do_sth_else...> : 
        flow<1/*不可以并发*/, do_sth, do_sth_else...> {
        friend struct utils;
    };

    template<auto do_sth, auto ... do_sth_else>
    struct can_also<true, do_sth, do_sth_else...> : 
        flow<2/*默认 2 个并发通道，支持再配置*/, do_sth, do_sth_else...> {
        friend struct utils;

        template<uxx value>
        using concurrency = flow<value, do_sth, do_sth_else...>;
    };

    template<auto do_something>
    struct when{
        template<auto ... do_something_else>
        using can = can_also<
            // 如果相同的函数可以重入，那么该函数就是可以并发的
            vin<vlist<do_something_else...>, do_something>,
            do_something, 
            do_something_else...
        >;
    };

    template<class ... rules>
    using pair_t = decltype(
        utils::build<rules...>()
    );

    template<class ... rules>
    using bits_t = typename pair_t<rules...>::bits_t;

    template<class bits_t, class ... rules>
    xstruct(
        xtmpl(policy_barrier_t, bits_t, rules...),
        xprif(state, bits_t)
    )
        using rule_list     = tlist<rules...>;
        using raw_data_list = typename pair_t<rules...>::new_list;

        policy_barrier_t() : state(0){}

        template<auto operation>
        uxx try_lock(){
            using raw           = tget<raw_data_list, uxx(operation)>;
            constexpr uxx mutex = ~share_mask<raw_data_list, typename raw::share_for>;

            // 可以执行相同的操作
            if constexpr (raw::can_concurrency) while(true){
                uxx index = index_of_first_set(~state & raw::mask);
                if (index >= raw::boundary){
                    return not_exist;
                }

                uxx candicate = uxx(1) << index;
                uxx old       = atom_fetch_or<bits_t>(xref state, bits_t(candicate));

                // 存在互斥操作
                if ((old & mutex) != 0){
                    if ((old & candicate) == 0) {
                        atom_and<bits_t>(xref state, bits_t(~candicate));
                    }
                    return not_exist;
                }
                // 当前位未被占用，则使用通道 candicate
                else if ((old & candicate) == 0){
                    return candicate;
                }
            }
            else{
                // 当前位被占用，无法操作
                uxx old = atom_fetch_or<bits_t>(xref state, bits_t(raw::master));
                if (old & raw::master){
                    return not_exist;
                }

                // 后判断互斥操作，因为此处的 operation 可以与自己互斥
                if (old & mutex){
                    atom_and<bits_t>(xref state, bits_t(~raw::master));
                    return not_exist;
                }
                else{
                    return raw::master;
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
            atom_and<bits_t>(xref state, bits_t(~channel));
        }
    $

    struct force_machine_word{};

    template<class first, class ... rules>
    inline auto configure(){
        if constexpr (is_same<force_machine_word, first>){
            return policy_barrier_t<uxx, rules...>();
        }
        else{
            return policy_barrier_t<
                bits_t<first, rules...>, 
                first, rules...
            >();
        }
    }

    template<class first, class ... rules>
    using policy_barrier = decltype(configure<first, rules...>());
}

namespace mixc::lock_policy_barrier::origin{
    using mixc::lock_policy_barrier::policy_barrier;
    using mixc::lock_policy_barrier::force_machine_word;
    using mixc::lock_policy_barrier::when;
}

#endif

namespace xuser::inc{
    using namespace ::mixc::lock_policy_barrier::origin;
}
