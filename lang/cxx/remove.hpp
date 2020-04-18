#ifndef xusing_lang_cxx
    #include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_remove
#define xpack_lang_cxx_remove
    #pragma push_macro("xuser")
    #pragma push_macro("xusing_lang_cxx")
        #undef  xusing_lang_cxx
        #undef  xuser
        #define xuser mixc::lang_cxx_remove
        #include"define/base_type.hpp"
        #include"interface/can_alloc.hpp"
        #include"lang/cxx.hpp"
        #include"macro/xindex_rollback.hpp"
        #include"memop/copy.hpp"
        #include"memop/swap.hpp"
    #pragma pop_macro("xusing_lang_cxx")
    #pragma pop_macro("xuser")

    namespace mixc::lang_cxx_remove{
        template<class item>
        struct core : inc::cxx<item> {
            using inc::cxx<item>::cxx;
            using the_t = core<item>;

            auto remove(ixx start, ixx endx, inc::can_alloc<item> alloc) const {
                xindex_rollback(the.length(), start);
                xindex_rollback(the.length(), endx);

                if (start > endx){
                    inc::swap(& start, & endx);
                }

                auto  len = the.length() - (endx - start);
                the_t r { alloc(len), len };
                inc::copy<item>(r        , the       , start);
                inc::copy<item>(r + start, the + endx, len - start);
                return r;
            }
        };
    }
#endif

namespace mixc::lang_cxx_remove::xuser{
    template<class final, class item>
    struct cxx : xusing_lang_cxx::cxx<final, item>{
        using xusing_lang_cxx::cxx<final, item>::cxx;
        using the_t = core<item>;

        final remove(ixx start, inc::can_alloc<item> alloc) const {
            return the.remove(start, -1, alloc);
        }

        final remove(ixx start, ixx endx, inc::can_alloc<item> alloc) const {
            return the.remove(start, endx, alloc);
        }
    };
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_remove::xuser
