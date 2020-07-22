#ifdef xuser
#ifndef xusing_lang_cxx
#include"lang/private/cxx.hpp"
#endif
#include"macro/xfinal.hpp"

namespace xuser::inc{
    template<class item>
    xfinal_tmpl(xusing_lang_cxx, cxx, item);

    using c08 = cxx<char>;
    using c16 = cxx<char16_t>;
}

#undef  xusing_lang_cxx
#endif
