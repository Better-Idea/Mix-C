#ifndef xusing_lang_cxx
    #include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_compare_fastly
#define xpack_lang_cxx_compare_fastly
    #pragma push_macro("xuser")
    #pragma push_macro("xusing_lang_cxx")
        #undef  xusing_lang_cxx
        #undef  xuser
        #define xuser mixc::lang_cxx_compare_fastly
        #include"define/base_type.hpp"
        #include"interface/can_compare.hpp"
        #include"lang/cxx.hpp"
    #pragma pop_macro("xusing_lang_cxx")
    #pragma pop_macro("xuser")

    namespace mixc::lang_cxx_compare_fastly{
        template<class item>
        struct core : inc::cxx<item> {
            using inc::cxx<item>::cxx;
            using the_t = core<item>;

            ixx compare_fastly(the_t value, inc::can_compare<item> compare) const {
                if (ixx r = ixx(the.length() - value.length()); r != 0){
                    return r;
                }
                for(uxx i = 0; i < the.length(); i++){
                    if (ixx r = compare(the[i], value[i]); r != 0){
                        return r;
                    }
                }
                return 0;
            }
        };
    }
#endif

namespace mixc::lang_cxx_compare_fastly::xuser{
    template<class final, class item>
    struct cxx : xusing_lang_cxx::cxx<final, item> {
        using xusing_lang_cxx::cxx<final, item>::cxx;
        using the_t = core<item>;

        ixx compare_fastly(
            final                  value, 
            inc::can_compare<item> compare = inc::default_compare<item>) const {
            return the.compare_fastly(value, compare);
        }
    };
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_compare_fastly::xuser
