#ifndef xusing_lang_cxx
    #include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_pad_right
#define xpack_lang_cxx_pad_right
    #pragma push_macro("xuser")
    #pragma push_macro("xusing_lang_cxx")
        #undef  xusing_lang_cxx
        #undef  xuser
        #define xuser mixc::lang_cxx_pad_right
        #include"define/base_type.hpp"
        #include"lang/cxx.hpp"
        #include"memop/copy.hpp"
        #include"memop/fill.hpp"
        #include"memory/alloc_callback.hpp"
    #pragma pop_macro("xusing_lang_cxx")
    #pragma pop_macro("xuser")

    namespace mixc::lang_cxx_pad_right{
        template<class item>
        xgc(core,
            xtmpl(item),
            xpub(inc::cxx<item>)
        )
            using inc::cxx<item>::cxx;
            using the_t = __self__;

            auto pad_right(uxx count, item value, inc::alloc_callback<item> alloc) const {
                uxx             length = the.length + count;
                the_t            r = { alloc(length), length };
                inc::copy<item>(r, the, the.length);
                inc::fill<item>(r.backward(the.length), value, count);
                return r;
            }
        xgc_end();
    }
#endif

namespace xuser::lang_cxx_pad_right{
    namespace cur{
        using namespace mixc::lang_cxx_pad_right;
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

        final pad_right(uxx count, item value, inc::alloc_callback<item> alloc) const {
            return the.pad_right(count, value, alloc);
        }
    xgc_end();
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx xuser::lang_cxx_pad_right
