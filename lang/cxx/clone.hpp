#ifndef xusing_lang_cxx
    #include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_clone
#define xpack_lang_cxx_clone
    #pragma push_macro("xuser")
    #pragma push_macro("xusing_lang_cxx")
        #undef  xuser
        #define xuser mixc::lang_cxx_clone
        #include"lang/cxx.hpp"
        #include"memop/copy.hpp"
        #include"memop/cast.hpp"
        #include"memory/alloc_callback.hpp"
    #pragma pop_macro("xusing_lang_cxx")
    #pragma pop_macro("xuser")

    namespace mixc::lang_cxx_clone{
        template<class item>
        struct cxx : inc::cxx<item>::partial{
            auto clone(inc::alloc_callback<item> alloc) const {
                inc::cxx<item> & self = xthe;
                inc::cxx<item>  r = { alloc(self.length), self.length };
                inc::copy<item>(r, self, self.length);
                return r;
            }
        };
    }
#endif

namespace xuser::lang_cxx_clone{
    namespace cur{
        using namespace mixc::lang_cxx_clone;
    }
    namespace inc{
        using namespace cur::inc;
    }
    template<class item, class final>
    struct cxx : xusing_lang_cxx::cxx<item, final> {
        using xusing_lang_cxx::cxx<item, final>::cxx;
        using fun = cur::cxx<item>;

        final clone(inc::alloc_callback<item> alloc) const {
            return inc::cast<fun>(xthe).clone(alloc);
        }
    };
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx xuser::lang_cxx_clone
