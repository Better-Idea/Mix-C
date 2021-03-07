// 注意：cxx<item_t> 主要用作字符串处理，目前仅支持 char/char16_t 两种元素类型
#ifdef  xuser
#ifndef xusing_lang_cxx
#include"lang/private/cxx.hpp"
#endif

namespace xuser{
    template<class item_t> struct cxx;

    template<>
    struct cxx<char> : xusing_lang_cxx::cxx<cxx<char>, char>{
        using xusing_lang_cxx::cxx<cxx<char>, char>::cxx;
    };

    template<>
    struct cxx<char16_t> : xusing_lang_cxx::cxx<cxx<char16_t>, char16_t>{
        using xusing_lang_cxx::cxx<cxx<char16_t>, char16_t>::cxx;
    };

    using c08 = cxx<char>;
    using c16 = cxx<char16_t>;
}

#undef  xusing_lang_cxx
#endif
