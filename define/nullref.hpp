#ifndef xpack_define_nullref_t
#define xpack_define_nullref_t
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::define_nullref_t
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::define_nullref_t{
    xstruct(
        xname(nullref_t)
    )
        template<class a> friend auto operator == (a const & value, nullref_t) {
            // 编译器假定绑定是不会绑定到空地址的
            // 所以使用 volatile 避免被优化(不加 volatile 优化器直接暴力移除该比较)
            volatile auto ptr = inc::addressof(value);
            return voidp(ptr) == nullptr;
        }

        template<class a> friend auto operator == (nullref_t, a const & value) {
            volatile auto ptr = inc::addressof(value);
            return voidp(ptr) == nullptr;
        }

        template<class a> friend auto operator != (a const & value, nullref_t) {
            volatile auto ptr = inc::addressof(value);
            return voidp(ptr) != nullptr;
        }

        template<class a> friend auto operator != (nullref_t, a const & value) {
            volatile auto ptr = inc::addressof(value);
            return voidp(ptr) != nullptr;
        }

        template<class a>
        operator a & () const {
            return *(a *)this;
        }
    $

    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wnull-dereference"
    static inline const nullref_t & nullref = *(nullref_t *)nullptr;
    #pragma GCC diagnostic pop
}

#endif

namespace xuser::inc{
    using ::mixc::define_nullref_t::nullref;
}
