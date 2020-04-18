#ifndef xusing_lang_cxx
    #include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_pad_left
#define xpack_lang_cxx_pad_left
    #pragma push_macro("xuser")
    #pragma push_macro("xusing_lang_cxx")
        #undef  xusing_lang_cxx
        #undef  xuser
        #define xuser mixc::lang_cxx_pad_left
        #include"define/base_type.hpp"
        #include"interface/can_alloc.hpp"
        #include"lang/cxx.hpp"
        #include"memop/copy.hpp"
        #include"memop/fill.hpp"
    #pragma pop_macro("xusing_lang_cxx")
    #pragma pop_macro("xuser")

    namespace mixc::lang_cxx_pad_left{
        template<class item>
        struct core : inc::cxx<item> {
            using inc::cxx<item>::cxx;
            using the_t = core<item>;

            auto pad_left(uxx count, item value, inc::can_alloc<item> alloc) const {
                uxx             length = the.length() + count;
                the_t           r(alloc(length), length);
                inc::fill<item>(r, value, count);
                inc::copy<item>(r.backward(count), the, the.length());
                return r;
            }
        };
    }
#endif

namespace mixc::lang_cxx_pad_left::xuser {
    template<class final, class item>
    struct cxx : xusing_lang_cxx::cxx<final, item> {
        using xusing_lang_cxx::cxx<final, item>::cxx;
        using the_t = core<item>;

        final pad_left(uxx count, item value, inc::can_alloc<item> alloc) const {
            return the.pad_left(count, value, alloc);
        }
    };
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_pad_left::xuser
