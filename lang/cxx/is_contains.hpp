#ifndef xusing_lang_cxx
    #include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_is_contains
#define xpack_lang_cxx_is_contains
    #pragma push_macro("xuser")
    #pragma push_macro("xusing_lang_cxx")
        #undef  xusing_lang_cxx
        #undef  xuser
        #define xuser mixc::lang_cxx_is_contains
        #include"define/base_type.hpp"
        #include"interface/can_compare.hpp"
        #include"interface/initializer_list.hpp"
        #include"lang/cxx/index_of_first.hpp"
        #include"lang/cxx.hpp"
    #pragma pop_macro("xusing_lang_cxx")
    #pragma pop_macro("xuser")
#endif

namespace mixc::lang_cxx_is_contains::xuser{
    template<class final, class item>
    struct cxx : xusing_lang_cxx::cxx<final, item>{
        using xusing_lang_cxx::cxx<final, item>::cxx;
        using the_t = inc::cxx<item>;

        bool is_contains(
            item                   value,
            inc::can_compare<item> compare = inc::default_compare<item>) const {
            return the.index_of_first(value, compare) != not_exist;
        }

        bool is_contains(
            inc::initializer_list<item> values,
            inc::can_compare<item>      compare = inc::default_compare<item>) const {
            return the.index_of_first(values.begin(), values.size(), compare) != not_exist;
        }

        bool is_contains(
            final                  value,
            inc::can_compare<item> compare = inc::default_compare<item>) const {
            return the.index_of_first(value, compare) != not_exist;
        }
    };
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_is_contains::xuser
