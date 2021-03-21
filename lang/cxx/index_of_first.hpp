#ifndef xusing_lang_cxx
#include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_index_of_first
#define xpack_lang_cxx_index_of_first
#pragma push_macro("xuser")
#pragma push_macro("xusing_lang_cxx")
#undef  xusing_lang_cxx
#undef  xuser
#define xuser mixc::lang_cxx_index_of_first::inc
#include"interface/can_callback.hpp"
#include"interface/can_compare.hpp"
#include"interface/initializer_list.hpp"
#include"interface/unified_seq.hpp"
#include"lang/cxx.hpp"
#include"macro/xlink.hpp"
#include"meta/has_cast.hpp"
#include"meta/item_origin_of.hpp"
#include"mixc.hpp"
#pragma pop_macro("xusing_lang_cxx")
#pragma pop_macro("xuser")

namespace mixc::lang_cxx_index_of_first{
    template<class item_t>
    struct core : inc::cxx<item_t> {
        using the_t     = core<item_t>;
        using base_t    = inc::cxx<item_t>;
        using base_t::base_t;

        core(base_t const & self) : 
            base_t(self){}

        template<class seq_t, class cmp_t>
        uxx index_of_first_char(seq_t const & seq, cmp_t const & compare) const {
            for(uxx i = 0; i < the.length(); i++){
                for(uxx ii = 0; ii < seq.length(); ii++){
                    if (compare(the[i], seq[ii]) == 0){
                        return i;
                    }
                }
            }
            return not_exist;
        }

        template<class compare_invoke>
        uxx index_of_first(the_t value, compare_invoke const & compare) const {
            the_t origin = the;
            uxx   miss   = 0;
            uxx   index;

            if (origin.length() < value.length() or value.length() == 0) {
                return not_exist;
            }
            while(true){
                if (index = origin.index_of_first_char(the_t{xref value[miss], 1}, compare); 
                    index == not_exist){
                    break;
                }
                if (origin = origin.forward(miss - index); origin.length() < value.length()) {
                    break;
                }
                for(index = 0; ; index++){
                    if (index == value.length()) {
                        return uxx((item_t *)(origin) - (item_t *)(the));
                    }
                    if (compare(origin[index], value[index]) != 0) {
                        origin = origin.backward(miss);
                        miss = index;
                        break;
                    }
                }
            }
            return not_exist;
        }
    };

    #define xa_name     index_of_first
    #include"lang/cxx/private/xgen.index_of.hpp"
}

#endif

namespace mixc::lang_cxx_index_of_first::xuser {
    template<class final_t, class item_t>
    using cxx = meta<final_t, xusing_lang_cxx::cxx<final_t, item_t>, item_t>;
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_index_of_first::xuser
