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
        #include"lang/cxx.hpp"
        #include"memop/copy.hpp"
        #include"memop/fill.hpp"
        #include"memory/alloc_callback.hpp"
    #pragma pop_macro("xusing_lang_cxx")
    #pragma pop_macro("xuser")

    namespace mixc::lang_cxx_pad_left{
        template<class item>
        xgc(core,
            xtmpl(item),
            xpub(inc::cxx<item>)
        )
            using inc::cxx<item>::cxx;
            using the_t = __self__;

            auto pad_left(uxx count, item value, inc::alloc_callback<item> alloc) const {
                uxx             length = the.length + count;
                the_t            r(alloc(length), length);
                inc::fill<item>(r, value, count);
                inc::copy<item>(r.backward(count), the, the.length);
                return r;
            }
        xgc_end();
    }
#endif

namespace xuser::lang_cxx_pad_left{
    namespace cur{
        using namespace mixc::lang_cxx_pad_left;
    }
    namespace inc{
        using namespace cur::inc;
    }

    template<class item, class final>
    xgc(cxx,  
        xtmpl(item, final),
        xpub(xusing_lang_cxx::cxx<item, final>)
    )
        using xusing_lang_cxx::cxx<item, final>::cxx;
        using the_t = cur::core<item>;

        final pad_left(uxx count, item value, inc::alloc_callback<item> alloc) const {
            return the.pad_left(count, value, alloc);
        }
    xgc_end();
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx xuser::lang_cxx_pad_left
