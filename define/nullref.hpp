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
        template<class a> auto operator == (a const & value) const {
            auto ptr = inc::addressof(value);
            return voidp(ptr) == this;
        }

        template<class a> friend auto operator == (a const & value, nullref_t const & nullref) {
            return nullref == value;
        }

        template<class a> friend auto operator != (a const & value, nullref_t const & nullref) {
            return not (nullref == value);
        }

        template<class a> friend auto operator != (nullref_t const & nullref, a const & value) {
            return not (nullref == value);
        }

        template<class a>
        operator a & () const {
            return *(a *)this;
        }
    $

    static inline const nullref_t & nullref = (nullref_t &) *(volatile nullref_t *)nullptr;
}

#endif

namespace xuser::inc{
    using ::mixc::define_nullref_t::nullref;
}
