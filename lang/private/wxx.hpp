#ifndef xpack_lang_wxx
#define xpack_lang_wxx
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::lang_wxx
#include"define/base_type.hpp"
#include"macro/xstruct.hpp"
#pragma pop_macro("xuser")

namespace mixc::lang_wxx{
    template<class final, class type>
    xstruct(
        xtmpl(wxx, final, type),
        xprof(data, type)
    )
        wxx() = default;
        wxx(wxx const &) = default;
        wxx(type value) : data(value){}

        template<class final_t>
        wxx(wxx<final_t, type> const & self) :
            wxx((the_t &)self) {
        }

        operator type & (){
            return data;
        }
    $
}

#endif

#define xusing_lang_wxx     ::mixc::lang_wxx
