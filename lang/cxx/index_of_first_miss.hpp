#ifndef xusing_lang_cxx
    #include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_index_of_first_miss
#define xpack_lang_cxx_index_of_first_miss
    #pragma push_macro("xuser")
    #pragma push_macro("xusing_lang_cxx")
    #undef  xusing_lang_cxx
    #undef  xuser
    #define xuser mixc::lang_cxx_index_of_first_miss
    #include"define/base_type.hpp"
    #include"dumb/implicit.hpp"
    #include"interface/can_compare.hpp"
    #include"interface/initializer_list.hpp"
    #include"lang/cxx/index_of_first.hpp"
    #include"lang/cxx.hpp"

    namespace xuser{
        template<class item>
        struct core : inc::cxx<item> {
            using base_t = inc::cxx<item>;
            using base_t::base_t;
            using the_t = core<item>;

            core(base_t const & self) : 
                base_t(self){}

            uxx index_of_first_miss(item const * values, uxx length, inc::can_compare<item> compare) const {
                the_t token{ values, length };

                for (uxx i = 0; i < the.length(); i++){
                    if (token.index_of_first(the[i], compare) == not_exist){
                        return i;
                    }
                }
                return not_exist;
            }
        };
    }

    #pragma pop_macro("xusing_lang_cxx")
    #pragma pop_macro("xuser")
#endif

namespace mixc::lang_cxx_index_of_first_miss::xuser {
    template<class final, class item>
    struct cxx : xusing_lang_cxx::cxx<final, item> {
        using xusing_lang_cxx::cxx<final, item>::cxx;
        using the_t = core<item>;

        uxx index_of_first_miss(
            item                   value, 
            inc::can_compare<item> compare = inc::default_compare<item>) const {
            return the.index_of_first_miss(& value, 1, compare);
        }

        uxx index_of_first_miss(
            inc::initializer_list<item> values, 
            inc::can_compare<item>      compare = inc::default_compare<item>) const {
            return the.index_of_first_miss(values.begin(), values.size(), compare);
        }
    };
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_index_of_first_miss::xuser
