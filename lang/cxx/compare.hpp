#ifndef xusing_lang_cxx
    #include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_compare
#define xpack_lang_cxx_compare
    #pragma push_macro("xuser")
    #pragma push_macro("xusing_lang_cxx")
    #undef  xusing_lang_cxx
    #undef  xuser
    #define xuser mixc::lang_cxx_compare
    #include"define/base_type.hpp"
    #include"interface/can_compare.hpp"
    #include"lang/cxx.hpp"
    #include"math/smaller.hpp"

    namespace xuser{
        template<class item>
        struct core : inc::cxx<item> {
            using base_t = inc::cxx<item>;
            using base_t::base_t;
            using the_t = core<item>;

            core(base_t const & self) : 
                base_t(self){}

            ixx compare(the_t value, inc::can_compare<item> compare) const {
                uxx len = inc::smaller(the.length(), value.length());

                for(uxx i = 0; i < len; i++){
                    if (ixx r = compare(the[i], value[i]); r != 0){
                        return r;
                    }
                }
                return ixx(the.length() - value.length());
            }
        };
    }

    #pragma pop_macro("xusing_lang_cxx")
    #pragma pop_macro("xuser")
#endif

namespace mixc::lang_cxx_compare::xuser{
    template<class final, class item>
    struct cxx : xusing_lang_cxx::cxx<final, item> {
        using xusing_lang_cxx::cxx<final, item>::cxx;
        using the_t = core<item>;

        ixx compare(
            final                  value, 
            inc::can_compare<item> compare = inc::default_compare<item>) const {
            return the.compare(value, compare);
        }
    };
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_compare::xuser
