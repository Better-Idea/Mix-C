#ifndef xusing_lang_cxx
    #include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_strcat
#define xpack_lang_cxx_strcat
    #pragma push_macro("xuser")
    #pragma push_macro("xusing_lang_cxx")
    #undef  xusing_lang_cxx
    #undef  xuser
    #define xuser mixc::lang_cxx_strcat
    #include"define/base_type.hpp"
    #include"interface/can_alloc.hpp"
    #include"interface/initializer_list.hpp"
    #include"lang/cxx.hpp"
    #include"meta/is_same.hpp"

    namespace xuser{
        template<class item>
        struct core : inc::cxx<item> {
            using inc::cxx<item>::cxx;
            using the_t = core<item>;

            auto strcat(the_t * list, uxx length, inc::can_alloc<item> alloc) const {
                uxx total_length = the.length();

                for(uxx i = 0; i < length; i++){
                    total_length += list[i].length();
                }

                auto ptr = alloc(total_length);
                auto set = [&](uxx length){
                    auto mem = ptr;
                    ptr     += length;
                    return mem;
                };

                the.clone(set);
                
                for(uxx i = 0; i < length; i++){
                    list[i].clone(set);
                }
                return the_t(ptr, total_length);
            }
        };
    }

    #pragma pop_macro("xusing_lang_cxx")
    #pragma pop_macro("xuser")
#endif

namespace mixc::lang_cxx_strcat::xuser{
    template<class final, class item>
    struct cxx : xusing_lang_cxx::cxx<final, item>{
        using xusing_lang_cxx::cxx<final, item>::cxx;
        using the_t = core<item>;

        final strcat(final values, inc::can_alloc<item> alloc) const {
            return the.strcat(& values, 1, alloc);
        }

        final strcat(inc::initializer_list<final> values, inc::can_alloc<item> alloc) const {
            return the.strcat(values.begin(), values.size(), alloc);
        }
    };
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_strcat::xuser
