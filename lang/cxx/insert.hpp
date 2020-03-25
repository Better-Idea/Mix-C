#ifndef xusing_lang_cxx
    #include"lang/private/cxx.hpp"
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
        #include"memop/copy.hpp"
        #include"memory/alloc_callback.hpp"
    #pragma pop_macro("xusing_lang_cxx")
    #pragma pop_macro("xuser")

    namespace mixc::lang_cxx_insert{
        template<class item>
        struct core : inc::cxx<item> {
            using inc::cxx<item>::cxx;
            using the_t = core<item>;

            auto insert(ixx index, the_t value, inc::alloc_callback<item> alloc) const {
                xindex_rollback(the.length(), index, + 1);
                uxx             target_length = the.length() + value.length();
                the_t           r { alloc(target_length), target_length };
                inc::copy<item>(r, the, index);
                inc::copy<item>(r.backward(index), value, value.length());
                inc::copy<item>(r.backward(index + value.length()), the.backward(index), the.length() - index);
                return r;
            }
        };
    }
#endif

namespace xuser::com::lang_cxx_insert{
    namespace cur{ using namespace mixc::lang_cxx_insert; }
    namespace inc{ using namespace cur::inc; }

    template<class final, class item>
    struct cxx : xusing_lang_cxx::cxx<final, item> {
        using xusing_lang_cxx::cxx<final, item>::cxx;
        using the_t = cur::core<item>;

        final insert(ixx index, final value, inc::alloc_callback<item> alloc) const {
            return the.insert(index, value, alloc);
        }
    };
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx xuser::com::lang_cxx_insert
