#ifndef xusing_lang_cxx
    #include"lang/cxx/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_insert
#define xpack_lang_cxx_insert
    #pragma push_macro("xuser")
    #pragma push_macro("xusing_lang_cxx")
        #undef  xusing_lang_cxx
        #undef  xuser
        #define xuser mixc::lang_cxx_insert
        #include"define/base_type.hpp"
        #include"lang/cxx.hpp"
        #include"macro/xindex_rollback.hpp"
        #include"memop/cast.hpp"
        #include"memop/copy.hpp"
        #include"memory/alloc_callback.hpp"
    #pragma pop_macro("xusing_lang_cxx")
    #pragma pop_macro("xuser")

    namespace mixc::lang_cxx_insert{
        template<class item>
        struct cxx : inc::cxx<item>::partial {
            auto insert(ixx index, inc::cxx<item> value, inc::alloc_callback<item> alloc) const {
                inc::cxx<item> & self = xthe;
                xindex_rollback(self.length, index, + 1);
                uxx             target_length = self.length + value.length;
                inc::cxx<item>  r { alloc(target_length), target_length };
                inc::copy<item>(r, self, index);
                inc::copy<item>(r.backward(index), value, value.length);
                inc::copy<item>(r.backward(index + value.length), self.backward(index), self.length - index);
                return r;
            }
        };
    }
#endif

namespace xuser::lang_cxx_insert{
    namespace cur{
        using namespace mixc::lang_cxx_insert;
    }
    namespace inc{
        using namespace cur::inc;
    }

    template<class item, class final>
    struct cxx : xusing_lang_cxx::cxx<item, final> {
        using xusing_lang_cxx::cxx<item, final>::cxx;

        final insert(ixx index, inc::cxx<item> value, inc::alloc_callback<item> alloc) const {
            return inc::cast<cur::cxx<item>>(xthe).insert(index, value, alloc);
        }
    };
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx xuser::lang_cxx_insert
