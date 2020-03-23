#ifndef xusing_lang_cxx
    #include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_substr
#define xpack_lang_cxx_substr
    #pragma push_macro("xuser")
    #pragma push_macro("xusing_lang_cxx")
    #undef  xusing_lang_cxx
        #undef  xuser
        #define xuser mixc::lang_cxx_substr
        #include"define/base_type.hpp"
        #include"lang/cxx.hpp"
        #include"macro/xindex_rollback.hpp"
        #include"memop/cast.hpp"
        #include"memory/alloc_callback.hpp"
    #pragma pop_macro("xusing_lang_cxx")
    #pragma pop_macro("xuser")

    namespace mixc::lang_cxx_substr{
        template<class item>
        struct cxx : inc::cxx<item>::partial {
            auto substr(ixx start, ixx end, inc::alloc_callback<item> alloc) const {
                inc::cxx<item> & self = xthe;

                if (self.length == 0) {
                    return inc::cxx<item>();
                }

                xindex_rollback(self.length, start);
                xindex_rollback(self.length, end);

                uxx    target_length;
                item * buf;
                item * temp;

                if (start <= end) {
                    target_length   = uxx(end - start + 1);
                    temp            = buf = alloc(target_length);
                    while(start <= end){
                        temp[0]     = self[start];
                        temp       += 1;
                        start      += 1;
                    }
                }
                else{
                    target_length   = uxx(start - end + 1);
                    temp            = buf = alloc(target_length);
                    while(start >= end){
                        temp[0]     = self[start];
                        temp       += 1;
                        start      -= 1;
                    }
                }
                return inc::cxx<item>(buf, target_length);
            }
        };
    }
#endif

namespace xuser::lang_cxx_substr{
    namespace cur{
        using namespace mixc::lang_cxx_substr;
    }
    namespace inc{
        using namespace cur::inc;
    }

    template<class item, class final>
    struct cxx : xusing_lang_cxx::cxx<item, final> {
        using xusing_lang_cxx::cxx<item, final>::cxx;
        using fun = cur::cxx<item>;

        final substr(ixx start, ixx end, inc::alloc_callback<item> alloc) const {
            return inc::cast<fun>(xthe).substr(start, end, alloc);
        }

        final substr(ixx start, inc::alloc_callback<item> alloc) const {
            return substr(start, -1, alloc);
        }
    };
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx xuser::lang_cxx_substr
