#ifdef xuser
#ifndef xusing_lang_wxx
#include"lang/private/wxx.hpp"
#endif
#include"macro/xfinal.hpp"

namespace xuser::inc{
    template<class item>
    xfinal_tmpl(xusing_lang_wxx, wxx, item);

    using w08 = wxx<char>;
    using w16 = wxx<char16_t>;
}

#undef  xusing_lang_wxx
#endif
