#ifndef xusing_lang_cxx
    #include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_index_of_last_miss
#define xpack_lang_cxx_index_of_last_miss
    #pragma push_macro("xuser")
    #pragma push_macro("xusing_lang_cxx")
        #undef  xusing_lang_cxx
        #undef  xuser
        #define xuser mixc::lang_cxx_index_of_last_miss
        #include"define/base_type.hpp"
        #include"dumb/implicit.hpp"
        #include"lang/cxx/index_of_first.hpp"
        #include"lang/cxx.hpp"
    #pragma pop_macro("xusing_lang_cxx")
    #pragma pop_macro("xuser")

    namespace mixc::lang_cxx_index_of_last_miss{
        template<class item>
        struct core : inc::cxx<item> {
            using inc::cxx<item>::cxx;
            using the_t = core<item>;

            auto index_of_last_miss(item const * value, uxx count) const {
                the_t token = { value, count };

                for (uxx i = the.length(); i--; ){
                    if (token.index_of_first(the[i]) == not_exist){
                        return i;
                    }
                }
                return not_exist;
            }
        };
    }
#endif

namespace mixc::lang_cxx_index_of_last_miss::xuser{
    template<class final, class item>
    struct cxx : xusing_lang_cxx::cxx<final, item> {
        using xusing_lang_cxx::cxx<final, item>::cxx;
        using the_t = core<item>;

        auto index_of_last_miss(item const * value, uxx count) const {
            return the.index_of_last_miss(value, count);
        }

        template<class ... args>
        auto index_of_last_miss(item value, args const & ... list) const {
            inc::implicit<item> group[] = { value, list... };
            return index_of_last_miss((item *)group, 1 + sizeof...(args));
        }
    };
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_index_of_last_miss::xuser
