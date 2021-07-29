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
//   when<do_sth_v>::can<do_sth_v, do_sth_else...>::concurrency<1>
// - 对于一个耗时较长且可以并发的同步区，可以给一个较大的并发值
#ifndef xpack_concurrency_lock_policy_barrier
#define xpack_concurrency_lock_policy_barrier
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::concurrency_lock_policy_barrier::inc
#include"concurrency/lock/atom_and.hpp"
#include"concurrency/lock/atom_fetch_or.hpp"
#include"concurrency/thread_self.hpp"
#include"instruction/index_of_first_set.hpp"
#include"macro/xexport.hpp"
#include"macro/xstruct.hpp"
#include"macro/xref.hpp"
#include"meta/fit_bits.hpp"
#include"meta/is_same.hpp"
#include"meta_seq/tget.hpp"
#include"meta_seq/tlist.hpp"
#include"meta_seq/vin.hpp"
#include"meta_seq/vlist.hpp"
#pragma pop_macro("xuser")

namespace mixc::concurrency_lock_policy_barrier{
    using namespace inc;

    template<class raw_data_list_t, class share_list_t>
    constexpr uxx share_mask = 0;

    template<class raw_data_list_t>
    constexpr uxx share_mask<raw_data_list_t, vlist<>> = 0;

    template<class raw_data_list_t, auto ... index_v>
    constexpr uxx share_mask<raw_data_list_t, vlist<index_v...>> = 
        (... | tget<raw_data_list_t, index_v>::mask);

    template<uxx offset_v, uxx max_con_v, class share_list_t>
    struct raw_data{
        enum : uxx{ 
            master          = uxx(1) << (offset_v),
            mask            = max_con_v == sizeof(uxx) * 8 ? 
                uxx(-1) : ((uxx(1) << max_con_v) - 1) << offset_v,
            boundary        = offset_v + max_con_v,
            can_concurrency = max_con_v > 1,
        };
        using share_for     = share_list_t;
    };

    template<uxx total_bits_v, class tlist_t>
    struct pair{
        using bits_t        = fit_bits<total_bits_v>;
        using new_list      = tlist_t;
    };

    struct utils{
    private:
        struct dummy{
            enum : uxx{ master = uxx(-1) };
        };

        template<class previous_t, class ... result_t>
        static auto make_order(previous_t, tlist<result_t...>, tlist<>){
            return tlist<result_t...>();
        }

        template<class previous_t, class first_t, class ... rest_t, class ... result_t>
        static auto make_order(previous_t, tlist<result_t...>, tlist<first_t, rest_t...>){
            // 构建按 master 升序排序
            if constexpr (previous_t::master + 1 == first_t::master){
                return make_order(first_t(), tlist<result_t..., first_t>(), tlist<rest_t...>());
            }
            else{
                return make_order(previous_t(), tlist<result_t...>(), tlist<rest_t..., first_t>());
            }
        }

        template<uxx offset_v, class first_t, class ... rest_t, class ... result_t>
        static auto tidy(tlist<result_t...>, tlist<first_t, rest_t...>){
            // 假如该成员函数的执行与其他成员函数都互斥（不能与其他操作同时执行）
            // 将同步域第 0 位分配给那些需要全局互斥的成员函数，否则需要分配位域用作同步
            constexpr auto can_share    = first_t::share_for::length != 0;
            constexpr auto current      = can_share ? offset_v : 0;
            constexpr auto new_offset   = can_share ? offset_v + first_t::max_concurrency : offset_v;

            using new_item              = raw_data<current, first_t::max_concurrency, typename first_t::share_for>;
            using new_list              = tlist<result_t..., new_item>;

            static_assert(new_offset <= sizeof(uxx) * 8);

            if constexpr (sizeof...(rest_t) == 0){
                return pair<new_offset, new_list>();
            }
            else{
                return tidy<new_offset>(new_list(), tlist<rest_t...>());
            }
        }
    public:
        template<class ... rules_t>
        static auto/*pair*/ build(){
            /*默认从第 1 位开始分配，第 0 位用作全局成员函数互斥位*/
            return tidy<1>(
                tlist<>(),
                make_order(dummy(), tlist<>(), tlist<rules_t...>())
            );
        }
    };

    template<auto max_con_v, auto do_sth_v, auto ... do_sth_else>
    struct flow{
    protected:
        friend struct utils;

        using share_for     = vlist<do_sth_else...>;
        enum{ 
            master          = do_sth_v, 
            max_concurrency = max_con_v,
        };
    };

    template<bool can_con, auto do_sth_v, auto ... do_sth_else>
    struct can_also;

    template<auto do_sth_v, auto ... do_sth_else>
    struct can_also<false, do_sth_v, do_sth_else...> : 
        flow<1/*该函数只能有一个线程进入，但支持其他线程执行其他函数 */, do_sth_v, do_sth_else...> {
        friend struct utils;
    };

    template<auto do_sth_v, auto ... do_sth_else>
    struct can_also<true, do_sth_v, do_sth_else...> : 
        flow<2/*默认 2 个并发通道，支持再配置*/, do_sth_v, do_sth_else...> {
        friend struct utils;

        // 再配置并发度
        template<uxx value>
        using concurrency = flow<value, do_sth_v, do_sth_else...>;
    };

    template<auto do_something>
    struct when{
        template<auto ... do_something_else>
        using can = can_also<
            // 如果 do_something 指示的函数可以被多个线程同时执行
            // 那么该 can<T> 模板还支持 ::concurrency<T> 子句模板用于配置该函数的最大并发度
            vin<vlist<do_something_else...>, do_something>,
            do_something, 
            do_something_else...
        >;
    };

    template<class ... rules_t>
    using pair_t = decltype(
        utils::build<rules_t...>()
    );

    template<class ... rules_t>
    using bits_t = typename pair_t<rules_t...>::bits_t;

    template<class bits_t, class ... rules_t>
    xstruct(
        xtmpl(policy_barrier_t, bits_t, rules_t...),
        xprif(m_state,  bits_t)
    )
        using rule_list     = tlist<rules_t...>;
        using raw_data_list = typename pair_t<rules_t...>::new_list;

        constexpr policy_barrier_t() : m_state(0){}

        template<auto operation_v>
        uxx try_lock() const {
            using raw           = tget<raw_data_list, uxx(operation_v)>;
            constexpr uxx mutex = ~share_mask<raw_data_list, typename raw::share_for>;

            // 可以执行相同的操作
            if constexpr (raw::can_concurrency) while(true){
                uxx index = index_of_first_set(~m_state & raw::mask);
                if (index >= raw::boundary){
                    return not_exist;
                }

                uxx candicate = uxx(1) << index;
                uxx old       = atom_fetch_or<bits_t>(xref(m_state), bits_t(candicate));

                // 存在互斥操作
                if ((old & mutex) != 0){
                    if ((old & candicate) == 0) {
                        atom_and<bits_t>(xref(m_state), bits_t(~candicate));
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
                uxx old = atom_fetch_or<bits_t>(xref(m_state), bits_t(raw::master));
                if (old & raw::master){
                    return not_exist;
                }

                // 后判断互斥操作，因为此处的 operation_v 可以与自己互斥
                if (old & mutex){
                    atom_and<bits_t>(xref(m_state), bits_t(~raw::master));
                    return not_exist;
                }
                else{
                    return raw::master;
                }
            }
        }

        template<auto operation_v>
        uxx lock() const {
            while(true){
                if (uxx channel = try_lock<operation_v>(); channel != not_exist){
                    return channel;
                }
                thread_self::yield();
            }
        }

        // 兼容 meta/is_builtin_lock 的接口
        template<auto operation_v, class this_t, class callback_t>
        void lock(this_t, callback_t const & call) const {
            lock<operation_v>(call);
        }

        template<auto operation_v, class callback_t>
        void lock(callback_t const & call) const {
            uxx channel = lock<operation_v>();
            call();
            unlock<operation_v>(channel);
        }

        template<auto operation_v>
        void unlock(uxx channel) const {
            atom_and<bits_t>(xref(m_state), bits_t(~channel));
        }
    $

    struct force_machine_word{};

    template<class first_t, class ... rules_t>
    inline auto configure(){
        if constexpr (is_same<force_machine_word, first_t>){
            return policy_barrier_t<uxx, rules_t...>();
        }
        else{
            return policy_barrier_t<
                bits_t<first_t, rules_t...>, 
                first_t, rules_t...
            >();
        }
    }

    template<class first_t, class ... rules_t>
    using policy_barrier = decltype(configure<first_t, rules_t...>());
}

namespace mixc::concurrency_lock_policy_barrier::origin{
    using mixc::concurrency_lock_policy_barrier::policy_barrier;
    using mixc::concurrency_lock_policy_barrier::force_machine_word;
    using mixc::concurrency_lock_policy_barrier::when;
}

#endif

xexport_space(mixc::concurrency_lock_policy_barrier::origin)