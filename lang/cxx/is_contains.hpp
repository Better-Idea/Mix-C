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
        #include"lang/cxx/index_of_first.hpp"
        #include"lang/cxx.hpp"
    #pragma pop_macro("xusing_lang_cxx")
    #pragma pop_macro("xuser")

    namespace mixc::lang_cxx_is_contains{
        template<class item>
        struct core : inc::cxx<item> {
            using inc::cxx<item>::cxx;
            using the_t = core<item>;

            bool is_contains(item value, inc::can_compare<item> compare) const {
                return the.index_of_first(value, compare) != not_exist;
            }

            bool is_contains(the_t value, inc::can_compare<item> compare) const {
                return the.index_of_first(value, compare) != not_exist;
            }
        };
    }
#endif

namespace mixc::lang_cxx_is_contains::xuser{
    template<class final, class item>
    struct cxx : xusing_lang_cxx::cxx<final, item>{
        using xusing_lang_cxx::cxx<final, item>::cxx;
        using the_t = core<item>;

        bool is_contains(
            item                   value,
            inc::can_compare<item> compare = inc::default_compare<item>) const {
            return the.is_contains(value, compare);
        }

        bool is_contains(
            final                  value,
            inc::can_compare<item> compare = inc::default_compare<item>) const {
            return the.is_contains(value, compare);
        }

        final & is_contains(
            item                   value,
            bool                 * receive,
            inc::can_compare<item> compare = inc::default_compare<item>) const {
            receive[0] = the.is_contains(value, compare);
            return thex;
        }

        final & is_contains(
            final                  value,
            bool                 * receive,
            inc::can_compare<item> compare = inc::default_compare<item>) const {
            receive[0] = the.is_contains(value, compare);
            return thex;
        }
    };
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_is_contains::xuser
