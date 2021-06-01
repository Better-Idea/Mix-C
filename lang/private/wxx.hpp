#ifndef xpack_lang_wxx
#define xpack_lang_wxx
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::lang_wxx::inc
#include"define/base_type.hpp"
#include"macro/xstruct.hpp"
#pragma pop_macro("xuser")

namespace mixc::lang_wxx{
    template<class final_t, class type>
    xstruct(
        xtmpl(wxx, final_t, type),
        xprif(m_value, type)
    )
        constexpr wxx() : m_value('\0'){};
        constexpr wxx(wxx const &) = default;
        constexpr wxx(type value) : m_value(value){}

        template<class finalx_t>
        wxx(wxx<finalx_t, type> const & self) :
            wxx((the_t &)self) {
        }

        operator type & (){
            return m_value;
        }
    $
}

#endif

#define xusing_lang_wxx     ::mixc::lang_wxx
