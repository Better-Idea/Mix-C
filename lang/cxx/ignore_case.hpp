#ifndef xpack_lang_cxx_ignore_case
#define xpack_lang_cxx_ignore_case
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::lang_cxx_ignore_case::inc
#include"define/base_type.hpp"
#include"lang/wxx/to_lower.hpp"
#include"lang/wxx.hpp"
#include"macro/xcmp.hpp"
#pragma pop_macro("xuser")

namespace mixc::lang_cxx_ignore_case{
    template<class item_t>
    inline auto ignore_case = 
        []xcmp(item_t) {
            return 
                inc::wxx<item_t>(left).to_lower() - 
                inc::wxx<item_t>(right).to_lower();
        };
}

#endif

xexport(mixc::lang_cxx_ignore_case::ignore_case)