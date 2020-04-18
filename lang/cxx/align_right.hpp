#ifndef xusing_lang_cxx
    #include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_align_right
#define xpack_lang_cxx_align_right
    #pragma push_macro("xuser")
    #pragma push_macro("xusing_lang_cxx")
        #undef  xusing_lang_cxx
        #undef  xuser
        #define xuser mixc::lang_cxx_align_right
        #include"define/base_type.hpp"
        #include"interface/can_alloc.hpp"
        #include"lang/cxx/clone.hpp"
        #include"lang/cxx/pad_left.hpp"
        #include"lang/cxx.hpp"
    #pragma pop_macro("xusing_lang_cxx")
    #pragma pop_macro("xuser")

    namespace mixc::lang_cxx_align_right{
        template<class item>
        struct core : inc::cxx<item> {
            using inc::cxx<item>::cxx;
            using the_t = core<item>;

            auto align_right(uxx width, item pad, inc::can_alloc<item> alloc) const {
                if (ixx r = ixx(the.length() - width); r >= 0){
                    return the.shorten(uxx(r)).clone(alloc);
                }
                else{
                    return the.pad_left(uxx(-r), pad, alloc);
                }
            }
        };
    }
#endif

namespace mixc::lang_cxx_align_right::xuser{
    template<class final, class item>
    struct cxx : xusing_lang_cxx::cxx<final, item> {
        using xusing_lang_cxx::cxx<final, item>::cxx;
        using the_t = core<item>;

        final align_right(uxx width, inc::can_alloc<item> alloc) const {
            return the.align_right(width, ' ', alloc);
        }

        final align_right(uxx width, item pad, inc::can_alloc<item> alloc) const {
            return the.align_right(width, pad, alloc);
        }
    };
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_align_right::xuser
