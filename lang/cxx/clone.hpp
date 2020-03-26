#ifndef xusing_lang_cxx
    #include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_clone
#define xpack_lang_cxx_clone
    #pragma push_macro("xuser")
    #pragma push_macro("xusing_lang_cxx")
        #undef  xuser
        #define xuser mixc::lang_cxx_clone
        #include"define/base_type.hpp"
        #include"lang/cxx.hpp"
        #include"macro/xgc.hpp"
        #include"memop/copy.hpp"
        #include"memory/alloc_callback.hpp"
    #pragma pop_macro("xusing_lang_cxx")
    #pragma pop_macro("xuser")

    namespace mixc::lang_cxx_clone{
        template<class item>
        struct core : inc::cxx<item> {
            using inc::cxx<item>::cxx;
            using the_t = core<item>;

            auto clone(inc::alloc_callback<item> alloc) const {
                the_t r = { alloc(the.length()), the.length() };
                inc::copy<item>(r, the, the.length());
                return r;
            }
        };
    }
#endif

namespace mixc::lang_cxx_clone::xuser {
    template<class final, class item>
    struct cxx : xusing_lang_cxx::cxx<final, item> {
        using xusing_lang_cxx::cxx<final, item>::cxx;
        using the_t = core<item>;

        final clone(inc::alloc_callback<item> alloc) const {
            return the.clone(alloc);
        }
    };
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_clone::xuser
