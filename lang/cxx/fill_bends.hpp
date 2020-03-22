#ifndef xusing_lang_cxx
    #include"lang/cxx/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_fill_bends
#define xpack_lang_cxx_fill_bends
    #pragma push_macro("xuser")
    #pragma push_macro("xusing_lang_cxx")
        #undef  xusing_lang_cxx
        #undef  xuser
        #define xuser mixc::lang_cxx_fill_bends
        #include"define/base_type.hpp"
        #include"lang/cxx.hpp"
        #include"memop/cast.hpp"
        #include"memop/copy.hpp"
        #include"memop/fill.hpp"
        #include"memory/alloc_callback.hpp"
    #pragma pop_macro("xusing_lang_cxx")
    #pragma pop_macro("xuser")

    namespace mixc::lang_cxx_fill_bends{
        template<class item>
        struct cxx : inc::cxx<item>::partial {
            auto fill_bends(uxx left_count, uxx right_count, item value, inc::alloc_callback<item> alloc) const {
                inc::cxx<item> & self = xthe;
                uxx              total_length = left_count + right_count + self.length;
                inc::cxx<item>   r(alloc(total_length), total_length);
                inc::fill<item>(r, value, left_count);
                inc::copy<item>(r.backward(left_count), self, self.length);
                inc::fill<item>(r.backward(left_count + self.length), value, right_count);
                return r;
            }
        };
    }
#endif

namespace xuser::lang_cxx_fill_bends{
    namespace cur{
        using namespace mixc::lang_cxx_fill_bends;
    }
    namespace inc{
        using namespace cur::inc;
    }

    template<class item, class final>
    struct cxx : xusing_lang_cxx::cxx<item, final> {
        using xusing_lang_cxx::cxx<item, final>::cxx;
        using fun = cur::cxx<item>;

        final fill_bends(uxx left_count, uxx right_count, item value, inc::alloc_callback<item> alloc) const {
            return inc::cast<fun>(xthe).fill_bends(left_count, right_count, value, alloc);
        }
    };
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx xuser::lang_cxx_fill_bends
