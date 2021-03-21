#pragma push_macro("xuser")
#undef  xuser
#define xuser xlink2(mixc::concurrency_lock_, xa_name)::inc
#include"configure.hpp"
#include"macro/xexport.hpp"
#include"macro/xlink.hpp"
#include"memop/addressof.hpp"
#include"meta/unsigned_type.hpp"
#pragma pop_macro("xuser")

namespace xlink2(mixc::concurrency_lock_, xa_name){
    #if xis_msvc_native // msvc 16.8 不支持模板内部声明外部函数
        extern u08 atom(u08p a, u08 b);
        extern u16 atom(u16p a, u16 b);
        extern u32 atom(u32p a, u32 b);

        #if xis_os32
        #else
        extern u64 atom(u64p a, u64 b);
        #endif
    #endif
}

namespace xlink2(mixc::concurrency_lock_, xa_name)::origin{
    template<class a0_t, class a1_t = a0_t>
    requires(sizeof(a0_t) <= sizeof(uxx) and sizeof(a1_t) <= sizeof(uxx))
    inline a0_t xa_name(a0_t * left, a1_t right){
        using u0_t  = inc::unsigned_type<a0_t>;
        using u1_t  = inc::unsigned_type<a1_t>;
        using up_t  = u0_t *;
        u0_t t;

        #if xis_msvc_native
            t       = atom(up_t(left), u0_t(*(u1_t *)(a1_t *)(xref right)));
        #else
            t       = xa_namex(up_t(left), u0_t(*(u1_t *)(a1_t *)(xref right)), 5/*最高级别限制的原子操作*/);
        #endif

        return *(a0_t *)& t;
    }
}

#undef  xa_name
#undef  xa_namex
