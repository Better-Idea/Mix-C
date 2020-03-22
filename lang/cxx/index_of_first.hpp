#ifndef xusing_lang_cxx
    #include"lang/cxx/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_index_of_first
#define xpack_lang_cxx_index_of_first
    #pragma push_macro("xuser")
    #pragma push_macro("xusing_lang_cxx")
        #undef  xusing_lang_cxx
        #undef  xuser
        #define xuser mixc::lang_cxx_index_of_first
        #include"define/base_type.hpp"
        #include"lang/cxx.hpp"
        #include"memop/cast.hpp"
    #pragma pop_macro("xusing_lang_cxx")
    #pragma pop_macro("xuser")

    namespace mixc::lang_cxx_index_of_first{
        template<class item>
        struct cxx : inc::cxx<item>::partial {
            auto index_of_first(item value) const {
                inc::cxx<item> & self = xthe;
                for (uxx i = 0; i < self.length; i++){
                    if (self[i] == value){
                        return i;
                    }
                }
                return not_exist;
            }
        };
    }
#endif

namespace xuser::lang_cxx_index_of_first{
    namespace cur{
        using namespace mixc::lang_cxx_index_of_first;
    }
    namespace inc{
        using namespace cur::inc;
    }

    template<class item, class final>
    struct cxx : xusing_lang_cxx::cxx<item, final> {
        using xusing_lang_cxx::cxx<item, final>::cxx;

        auto index_of_first(item value) const {
            return inc::cast<cur::cxx<item>>(xthe).index_of_first(value);
        }
    };
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx xuser::lang_cxx_index_of_first
