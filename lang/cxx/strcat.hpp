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
    #include"interface/can_alloc.hpp"
    #include"interface/initializer_list.hpp"
    #include"lang/cxx/clone.hpp"
    #include"lang/cxx.hpp"
    #include"mixc.hpp"
    #pragma pop_macro("xusing_lang_cxx")
    #pragma pop_macro("xuser")

    namespace mixc::lang_cxx_strcat{
        template<class item>
        struct core : inc::cxx<item> {
            using base_t = inc::cxx<item>;
            using base_t::base_t;
            using the_t = core<item>;

            core(base_t const & self) : 
                base_t(self){}

            auto strcat(the_t * list, uxx length, inc::can_alloc<item> alloc) const {
                uxx total_length = the.length();

                for(uxx i = 0; i < length; i++){
                    total_length += list[i].length();
                }

                auto ptr = alloc(total_length);
                auto tmp = ptr;
                auto set = [&](uxx length){
                    auto mem = tmp;
                    tmp     += length;
                    return mem;
                };

                the.clone(set);
                
                for(uxx i = 0; i < length; i++){
                    list[i].clone(set);
                }
                return the_t(ptr, total_length);
            }
        };

        template<class final, class base, class item>
        struct meta : base{
            using base::base;
            using the_t = core<item>;

            final strcat(final values, inc::can_alloc<item> alloc) const {
                return the.strcat((the_t *)(xref values), 1, alloc);
            }

            final strcat(inc::initializer_list<final> values, inc::can_alloc<item> alloc) const {
                return the.strcat((the_t *)values.begin(), values.size(), alloc);
            }
        };
    }

#endif

namespace mixc::lang_cxx_strcat::xuser{
    template<class final, class item>
    using cxx = meta<final, xusing_lang_cxx::cxx<final, item>, item>;
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_strcat::xuser
