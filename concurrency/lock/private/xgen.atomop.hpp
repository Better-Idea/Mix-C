#pragma push_macro("xuser")
#undef  xuser
#define xuser xlink2(mixc::concurrency_lock_, xa_name)::inc
#include"configure.hpp"
#include"macro/xexport.hpp"
#include"macro/xlink.hpp"
#include"meta/unsigned_type.hpp"
#include"meta/has_forced_cast.hpp"
#pragma pop_macro("xuser")

namespace xlink2(mixc::concurrency_lock_, xa_name){
    #if xis_msvc_native // msvc 16.8 不支持模板内部声明外部函数
        extern u64 xa_name(voidp a, u64 b, uxx bytes);
    #endif

    template<class a0_t, class a1_t = a0_t>
    requires(sizeof(a0_t) <= sizeof(uxx) and sizeof(a1_t) <= sizeof(uxx))
    inline a0_t xa_name(a0_t * left, a1_t right){
        using u0_t = inc::unsigned_type<a0_t>;
        using u1_t = inc::unsigned_type<a1_t>;
        using up_t = u0_t *;

        #if xis_msvc_native
            return (a0_t)xa_name(up_t(left), (u0_t)(a1_t)right, sizeof(a0_t));
        #else
            return (a0_t)xa_namex(up_t(left), (u0_t)(a1_t)right, 5/*最高级别限制的原子操作*/);
        #endif
    }
}

#undef  xa_name
#undef  xa_namex
