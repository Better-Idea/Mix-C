#ifndef xpack_docker_hashmap
#define xpack_docker_hashmap
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::docker_hashmap::inc
#include"algo/hash.hpp"
#include"define/base_type.hpp"
#include"define/nullref.hpp"
#include"docker/transmitter.hpp"
#include"dumb/mirror.hpp"
#include"dumb/disable_copy.hpp"
#include"gc/self_management.hpp"
#include"macro/xdefer.hpp"
#include"macro/xexport.hpp"
#include"macro/xitr_foreach.hpp"
#include"macro/xref.hpp"
#include"math/align.hpp"
#include"math/random.hpp"
#include"memop/copy.hpp"
#include"memop/cmp.hpp"
#include"memop/swap.hpp"
#include"meta/has_cmp_equal.hpp"
#include"meta/remove_ptr.hpp"
#include"utils/bits_indicator.hpp"
#include"utils/memory.hpp"
#pragma pop_macro("xuser")

namespace mixc::docker_hashmap{
    enum class hashmap_remove_result_t{
        success,
        item_not_exist,
    };

    enum class hashmap_take_out_result_t{
        success,
        item_not_exist,
    };

    enum class hashmap_set_result_t{
        success,
        override,
    };

    #define xarg_has_val_t
    #include"docker/private/xgen.hashmap.hpp"
    #include"docker/private/xgen.hashmap.hpp"

    /* 
     * struct __single_inheritance X;
     * 
     * struct A { };
     * struct B { };
     * struct X : A, B { };  // C2292, X uses multiple inheritance
     * 
     * C2292 的指示被声明单继承的类不能多继承，
     * 而 Mix-C 的项目从未使用该约束，但还是莫名奇妙的产生了该错误，并且暂时无法通过简化的代码模拟再现该错误
     * 所以只能拆成两个类(kvhashmap、khashmap) msvc 让作者心碎
     */
    template<class final_t, class key_t, class val_t>
    inline auto configure(final_t *, key_t *, val_t *) {
        return (kvhashmap<final_t, key_t, val_t> *)nullptr;
    }

    template<class final_t, class key_t>
    inline auto configure(final_t *, key_t *, voidp) {
        return (khashmap<final_t, key_t> *)nullptr;
    }

    template<class final_t, inc::has_cmp_equal key_t, class val_t>
    using hashmap = inc::remove_ptr<
        decltype(
            configure(
                (final_t *)nullptr,
                (key_t   *)nullptr,
                (val_t   *)nullptr
            )
        )
    >;
}

namespace mixc::docker_hashmap::origin{
    using ::mixc::docker_hashmap::hashmap_remove_result_t;
    using ::mixc::docker_hashmap::hashmap_take_out_result_t;
    using ::mixc::docker_hashmap::hashmap_set_result_t;
}

#endif

#define xusing_docker_hashmap     ::mixc::docker_hashmap

xexport_space(mixc::docker_hashmap::origin)
