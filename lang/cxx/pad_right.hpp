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
        #include"memop/cast.hpp"
        #include"memop/copy.hpp"
        #include"memop/fill.hpp"
        #include"memory/alloc_callback.hpp"
    #pragma pop_macro("xusing_lang_cxx")
    #pragma pop_macro("xuser")

    namespace mixc::lang_cxx_pad_right{
        template<class item>
        struct cxx : inc::cxx<item>::partial {
            auto pad_right(uxx count, item value, inc::alloc_callback<item> alloc) const {
                inc::cxx<item> & self = xthe;
                uxx              length = self.length + count;
                inc::cxx<item>   r = { alloc(length), length };
                inc::copy<item>(r, self, self.length);
                inc::fill<item>(r.backward(self.length), value, count);
                return r;
            }
        };
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
    struct cxx : xusing_lang_cxx::cxx<item, final> {
        using xusing_lang_cxx::cxx<item, final>::cxx;
        using fun = cur::cxx<item>;

        final pad_right(uxx count, item value, inc::alloc_callback<item> alloc) const {
            return inc::cast<fun>(xthe).pad_right(count, value, alloc);
        }
    };
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx xuser::lang_cxx_pad_right
