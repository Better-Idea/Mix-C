#ifndef xusing_lang_cxx
    #include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_align_left
#define xpack_lang_cxx_align_left
    #pragma push_macro("xuser")
    #pragma push_macro("xusing_lang_cxx")
        #undef  xusing_lang_cxx
        #undef  xuser
        #define xuser mixc::lang_cxx_align_left
        #include"define/base_type.hpp"
        #include"lang/cxx/clone.hpp"
        #include"lang/cxx/pad_right.hpp"
        #include"lang/cxx.hpp"
        #include"memory/alloc_callback.hpp"
    #pragma pop_macro("xusing_lang_cxx")
    #pragma pop_macro("xuser")

    namespace mixc::lang_cxx_align_left{
        template<class item>
        struct core : inc::cxx<item> {
            using inc::cxx<item>::cxx;
            using the_t = core<item>;

            the_t align_left(uxx width, item pad, inc::alloc_callback<item> alloc) const {
                if (ixx r = ixx(the.length() - width); r >= 0){
                    return the.shorten(uxx(r)).clone(alloc);
                }
                else{
                    return the.pad_right(uxx(-r), pad, alloc);
                }
            }
        };
    }
#endif

namespace mixc::lang_cxx_align_left::xuser{
    template<class final, class item>
    struct cxx : xusing_lang_cxx::cxx<final, item> {
        using xusing_lang_cxx::cxx<final, item>::cxx;
        using the_t = core<item>;

        final align_left(uxx width, inc::alloc_callback<item> alloc) const {
            return the.align_left(width, ' ', alloc);
        }

        final align_left(uxx width, item pad, inc::alloc_callback<item> alloc) const {
            return the.align_left(width, pad, alloc);
        }
    };
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_align_left::xuser
