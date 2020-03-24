#ifndef xusing_lang_cxx
    #include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_pad_bends
#define xpack_lang_cxx_pad_bends
    #pragma push_macro("xuser")
    #pragma push_macro("xusing_lang_cxx")
        #undef  xusing_lang_cxx
        #undef  xuser
        #define xuser mixc::lang_cxx_pad_bends
        #include"define/base_type.hpp"
        #include"lang/cxx.hpp"
        #include"memop/copy.hpp"
        #include"memop/fill.hpp"
        #include"memory/alloc_callback.hpp"
    #pragma pop_macro("xusing_lang_cxx")
    #pragma pop_macro("xuser")

    namespace mixc::lang_cxx_pad_bends{
        template<class item>
        xgc(core,
            xtmpl(item),
            xpub(inc::cxx<item>)
        )
            using inc::cxx<item>::cxx;
            using the_t = __self__;

            auto pad_bends(uxx left_count, uxx right_count, item value, inc::alloc_callback<item> alloc) const {
                uxx             total_length = left_count + right_count + the.length;
                the_t            r(alloc(total_length), total_length);
                inc::fill<item>(r, value, left_count);
                inc::copy<item>(r.backward(left_count), the, the.length);
                inc::fill<item>(r.backward(left_count + the.length), value, right_count);
                return r;
            }
        xgc_end();
    }
#endif

namespace xuser::lang_cxx_pad_bends{
    namespace cur{
        using namespace mixc::lang_cxx_pad_bends;
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

        final pad_bends(uxx left_count, uxx right_count, item value, inc::alloc_callback<item> alloc) const {
            return the.pad_bends(left_count, right_count, value, alloc);
        }
    xgc_end();
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx xuser::lang_cxx_pad_bends
