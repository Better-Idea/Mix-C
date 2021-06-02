#ifndef xusing_lang_cxx
#include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_index_of_last
#define xpack_lang_cxx_index_of_last
#pragma push_macro("xuser")
#pragma push_macro("xusing_lang_cxx")
#undef  xusing_lang_cxx
#undef  xuser
#define xuser mixc::lang_cxx_index_of_last::inc
#include"interface/can_callback.hpp"
#include"interface/can_compare.hpp"
#include"interface/initializer_list.hpp"
#include"interface/unified_seq.hpp"
#include"lang/cxx.hpp"
#include"macro/xlink.hpp"
#include"macro/xref.hpp"
#include"meta/has_cast.hpp"
#include"meta/item_origin_of.hpp"
#pragma pop_macro("xusing_lang_cxx")
#pragma pop_macro("xuser")

namespace mixc::lang_cxx_index_of_last{
    template<class item_t>
    struct core : inc::cxx<item_t> {
        using the_t             = core<item_t>;
        using base_t            = inc::cxx<item_t>;
        using base_t::base_t;

        core(base_t const & self) : 
            base_t(self){}

        template<class seq_t, class cmp_t>
        uxx index_of_last_char(seq_t const & seq, cmp_t const & compare) const {
            for(uxx i = the.length(); i--; ){
                for(uxx ii = 0; ii < seq.length(); ii++){
                    if (compare(the[i], seq[ii]) == 0){
                        return i;
                    }
                }
            }
            return not_exist;
        }

        template<class cmp_t>
        uxx index_of_last(the_t value, cmp_t const & compare) const {
            // miss 为未匹配项的索引
            // 每次都去匹配上一次未匹配的字符 str[miss]
            // 每当在 index 处匹配，就将原串缩短 index - miss + 1
            //      剩余可匹配的串
            //            |
            //         |--+--|
            // origin "123451235"
            // value         "34"
            the_t   origin = the;
            the_t   temp   = the;
            uxx     miss   = 0;
            uxx     index;

            if (origin.length() < value.length() or value.length() == 0) {
                return not_exist;
            }
            for (origin = origin.shorten(value.length() - 1);;){
                if (index = origin.index_of_last_char(the_t{xref(value[miss]), 1}, compare); 
                    index == not_exist){
                    break;
                }

                origin.length(index - miss);
                temp          = origin.backward(origin.length());
                origin        = origin.elongate(1);
                
                for (index = 0; ; index++){
                    if (index == value.length()) {
                        return origin.length() - 1;
                    }
                    if (compare(temp[index], value[index]) != 0) {
                        miss = index;
                        break;
                    }
                }
            }
            return not_exist;
        }
    };

    #define xa_name     index_of_last
    #include"lang/cxx/private/xgen.index_of.hpp"
}

#endif

namespace mixc::lang_cxx_index_of_last::xuser {
    template<class final_t, class item_t>
    using cxx = meta<final_t, xusing_lang_cxx::cxx<final_t, item_t>, item_t>;
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_index_of_last::xuser
