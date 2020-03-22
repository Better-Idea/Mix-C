#ifndef xusing_lang_cxx
    #include"lang/cxx/cxx.hpp"
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
        #include"lang/cxx/index_of_first.hpp"
        #include"lang/cxx.hpp"
        #include"memop/cast.hpp"
    #pragma pop_macro("xusing_lang_cxx")
    #pragma pop_macro("xuser")

    namespace mixc::lang_cxx_index_of_first_miss{
        template<class item>
        struct cxx : inc::cxx<item>::partial {
            auto index_of_first_miss(item const * values, uxx count){
                inc::cxx<item> & self = xthe;
                inc::cxx<item>   token = { values, count };

                for (uxx i = 0; i < self.length; i++){
                    if (token.index_of_first(self[i]) == not_exist){
                        return i;
                    }
                }
                return not_exist;
            }
        };
    }
#endif

namespace xuser::lang_cxx_index_of_first_miss{
    namespace cur{
        using namespace mixc::lang_cxx_index_of_first_miss;
    }
    namespace inc{
        using namespace cur::inc;
    }

    template<class item, class final>
    struct cxx : xusing_lang_cxx::cxx<item, final> {
        using xusing_lang_cxx::cxx<item, final>::cxx;
        using fun = cur::cxx<item>;

        auto index_of_first_miss(item const * values, uxx count) const {
            return inc::cast<fun>(xthe).index_of_first_miss(values, count);
        }

        template<class ... args>
        auto index_of_first_miss(item value, args const & ... list){
            inc::implicit<item> group[] = { value, list... };
            return index_of_first_miss((item *)group, 1 + sizeof...(args));
        }
    };
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx xuser::lang_cxx_index_of_first_miss
