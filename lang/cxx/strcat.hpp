#ifndef xusing_lang_cxx
#include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_strcat
#define xpack_lang_cxx_strcat
#pragma push_macro("xuser")
#pragma push_macro("xusing_lang_cxx")
#undef  xusing_lang_cxx
#undef  xuser
#define xuser mixc::lang_cxx_strcat::inc
#include"define/base_type.hpp"
#include"interface/can_alloc.hpp"
#include"interface/initializer_list.hpp"
#include"lang/cxx/clone.hpp"
#include"lang/cxx.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xusing_lang_cxx")
#pragma pop_macro("xuser")

namespace mixc::lang_cxx_strcat{
    template<class item_t>
    struct core {
        using the_t = inc::cxx<item_t>;

        template<class alloc_t>
        auto strcat(void const * list_ptr, uxx length, alloc_t const & alloc) const {
            the_t *     list            = (the_t *)list_ptr;
            uxx         total_length    = the.length();

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

    template<class final_t, class base_t, class item_t>
    struct meta : base_t{
        using base_t::base_t;
        using the_t = core<item_t>;

        template<class alloc_t>
        requires(
            inc::can_alloc<alloc_t, item_t>
        )
        final_t strcat(final_t values, alloc_t const & alloc) const {
            return the.strcat(& values, 1, alloc);
        }

        template<class alloc_t>
        requires(
            inc::can_alloc<alloc_t, item_t>
        )
        final_t strcat(inc::initializer_list<final_t> values, alloc_t const & alloc) const {
            return the.strcat(values.begin(), values.size(), alloc);
        }
    };
}

#endif

namespace mixc::lang_cxx_strcat::xuser{
    template<class final_t, class item_t>
    using cxx = meta<final_t, xusing_lang_cxx::cxx<final_t, item_t>, item_t>;
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_strcat::xuser
