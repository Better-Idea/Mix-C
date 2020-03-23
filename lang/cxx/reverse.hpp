#ifndef xusing_lang_cxx
    #include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_reverse
#define xpack_lang_cxx_reverse
    #pragma push_macro("xuser")
    #pragma push_macro("xusing_lang_cxx")
        #undef  xusing_lang_cxx
        #undef  xuser
        #define xuser mixc::lang_cxx_reverse
        #include"define/base_type.hpp"
        #include"lang/cxx/substr.hpp"
        #include"lang/cxx.hpp"
        #include"memop/cast.hpp"
        #include"memory/alloc_callback.hpp"
    #pragma pop_macro("xusing_lang_cxx")
    #pragma pop_macro("xuser")

    namespace mixc::lang_cxx_reverse{
        template<class item>
        struct cxx : inc::cxx<item>::partial {
            auto reverse(inc::alloc_callback<item> alloc) const {
                auto & self = inc::cast<inc::cxx<item>>(xthe);
                return self.substr(-1, 0, alloc);
            }
        };
    }
#endif

namespace xuser::lang_cxx_reverse{
    namespace cur{
        using namespace mixc::lang_cxx_reverse;
    }
    namespace inc{
        using namespace cur::inc;
    }

    template<class item, class final>
    struct cxx : xusing_lang_cxx::cxx<item, final> {
        using xusing_lang_cxx::cxx<item, final>::cxx;

        final reverse(inc::alloc_callback<item> alloc) const {
            return inc::cast<cur::cxx<item>>(xthe).reverse(alloc);
        }
    };
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx xuser::lang_cxx_reverse
