// 注意：wxx<item_t> 主要用作字符处理，目前仅支持 char/char16_t 两种元素类型
#ifdef xuser
#ifndef xusing_lang_wxx
#include"lang/private/wxx.hpp"
#endif

namespace xuser::inc{
    template<class item_t> struct wxx;

    template<>
    struct wxx<char> : xusing_lang_wxx::wxx<wxx<char>, char>{
        using xusing_lang_wxx::wxx<wxx<char>, char>::wxx;
    };

    template<>
    struct wxx<char16_t> : xusing_lang_wxx::wxx<wxx<char16_t>, char16_t>{
        using xusing_lang_wxx::wxx<wxx<char16_t>, char16_t>::wxx;
    };

    using w08 = wxx<char>;
    using w16 = wxx<char16_t>;
}

#undef  xusing_lang_wxx
#endif
