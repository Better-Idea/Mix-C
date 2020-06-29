#ifndef xusing_lang_cxx
    #include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_align_center
#define xpack_lang_cxx_align_center
    #pragma push_macro("xuser")
    #pragma push_macro("xusing_lang_cxx")
    #undef  xusing_lang_cxx
    #undef  xuser
    #define xuser mixc::lang_cxx_align_center
    #include"define/base_type.hpp"
    #include"interface/can_alloc.hpp"
    #include"lang/cxx/clone.hpp"
    #include"lang/cxx/pad_bends.hpp"
    #include"lang/cxx.hpp"

    namespace xuser{
        template<class item>
        struct core : inc::cxx<item> {
            using base_t = inc::cxx<item>;
            using base_t::base_t;
            using the_t = core<item>;

            core(base_t const & self) : 
                base_t(self){}

            auto align_center(uxx width, item pad, inc::can_alloc<item> alloc) const {
                if (ixx r = ixx(the.length() - width); r >= 0){
                    return the.shorten(uxx(r)).clone(alloc);
                }
                else{
                    uxx w     = uxx(-r);
                    uxx left  = w / 2;
                    uxx right = w - left;
                    return the.pad_bends(left, right, pad, alloc);
                }
            }
        };
    }

    #pragma pop_macro("xusing_lang_cxx")
    #pragma pop_macro("xuser")
#endif

namespace mixc::lang_cxx_align_center::xuser{
    template<class final, class item>
    struct cxx : xusing_lang_cxx::cxx<final, item> {
        using xusing_lang_cxx::cxx<final, item>::cxx;
        using the_t = core<item>;

        final align_center(uxx width, inc::can_alloc<item> alloc) const {
            return the.align_center(width, ' ', alloc);
        }

        final align_center(uxx width, item pad, inc::can_alloc<item> alloc) const {
            return the.align_center(width, pad, alloc);
        }
    };
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_align_center::xuser
