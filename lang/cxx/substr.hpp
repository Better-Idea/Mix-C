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
        #include"memory/alloc_callback.hpp"
    #pragma pop_macro("xusing_lang_cxx")
    #pragma pop_macro("xuser")

    namespace mixc::lang_cxx_substr{
        template<class item>
        struct core : inc::cxx<item> {
            using inc::cxx<item>::cxx;
            using the_t = core<item>;

            the_t substr(ixx start, ixx end, inc::alloc_callback<item> alloc) const {
                if (the.is_empty()) {
                    return the_t();
                }

                xindex_rollback(the.length(), start);
                xindex_rollback(the.length(), end);

                uxx    target_length;
                item * buf;
                item * temp;

                if (start <= end) {
                    target_length   = uxx(end - start + 1);
                    temp            = buf = alloc(target_length);
                    while(start <= end){
                        temp[0]     = the[start];
                        temp       += 1;
                        start      += 1;
                    }
                }
                else{
                    target_length   = uxx(start - end + 1);
                    temp            = buf = alloc(target_length);
                    while(start >= end){
                        temp[0]     = the[start];
                        temp       += 1;
                        start      -= 1;
                    }
                }
                return the_t(buf, target_length);
            }
        };
    }
#endif

namespace mixc::lang_cxx_substr::xuser {
    template<class final, class item>
    struct cxx : xusing_lang_cxx::cxx<final, item> {
        using xusing_lang_cxx::cxx<final, item>::cxx;
        using the_t = core<item>;

        final substr(ixx start, ixx end, inc::alloc_callback<item> alloc) const {
            return the.substr(start, end, alloc);
        }

        final substr(ixx start, inc::alloc_callback<item> alloc) const {
            return substr(start, -1, alloc);
        }
    };
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_substr::xuser
