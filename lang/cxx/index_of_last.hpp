#ifndef xusing_lang_cxx
    #include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_index_of_last
#define xpack_lang_cxx_index_of_last
    #pragma push_macro("xuser")
    #pragma push_macro("xusing_lang_cxx")
        #undef  xusing_lang_cxx
        #undef  xuser
        #define xuser mixc::lang_cxx_index_of_last
        #include"define/base_type.hpp"
        #include"interface/can_callback.hpp"
        #include"interface/can_compare.hpp"
        #include"lang/cxx.hpp"
    #pragma pop_macro("xusing_lang_cxx")
    #pragma pop_macro("xuser")

    namespace mixc::lang_cxx_index_of_last{
        template<class item>
        struct core : inc::cxx<item> {
            using inc::cxx<item>::cxx;
            using the_t = core<item>;

            uxx index_of_last(item value, inc::can_compare<item> compare) const {
                for (uxx i = the.length(); i--; ){
                    if (compare(the[i], value) == 0){
                        return i;
                    }
                }
                return not_exist;
            }

            template<class compare_invoke>
            uxx index_of_last(
                the_t                  substr,
                compare_invoke const & compare) const {

                // miss 为未匹配项的索引
                // 每次都去匹配上一次未匹配的字符 str[miss]
                // 每当在 index 处匹配，就将原串缩短 index - miss + 1
                //      剩余可匹配的串
                //            |
                //         |--+--|
                // origin "123451235"
                // substr        "34"
                the_t origin = the;
                the_t temp   = the;
                uxx   miss   = 0;
                uxx   index;

                if (origin.length() < substr.length() or substr.length() == 0) {
                    return not_exist;
                }
                for (origin = origin.shorten(substr.length() - 1);;){
                    if (index = origin.index_of_last(substr[miss], compare); index == not_exist){
                        break;
                    }

                    origin.length(index - miss);
                    temp          = origin.backward(origin.length());
                    origin        = origin.elongate(1);
                    
                    for (index = 0; ; index++){
                        if (index == substr.length()) {
                            return origin.length() - 1;
                        }
                        if (compare(temp[index], substr[index]) != 0) {
                            miss = index;
                            break;
                        }
                    }
                }
                return not_exist;
            }

            void index_of_last(
                the_t                              value, 
                inc::can_callback<void(uxx index)> match,
                inc::can_compare<item>             compare) const {

                for(auto cur = the;;){
                    if (uxx i = cur.index_of_last(value, compare); i == not_exist){
                        return;
                    }
                    else{
                        cur = cur.length(i);
                        match(i);
                    }
                }
            }
        };
    }
#endif

namespace mixc::lang_cxx_index_of_last::xuser {
    template<class final, class item>
    struct cxx : xusing_lang_cxx::cxx<final, item> {
        using xusing_lang_cxx::cxx<final, item>::cxx;
        using the_t = core<item>;

        uxx index_of_last(
            item                   value, 
            inc::can_compare<item> compare = inc::default_compare<item>) const {
            return the.index_of_last(value, compare);
        }

        uxx index_of_last(
            final                  value, 
            inc::can_compare<item> compare = inc::default_compare<item>) const {
            return the.index_of_last(value, compare);
        }

        void index_of_last(
            the_t                              value, 
            inc::can_callback<void(uxx index)> match,
            inc::can_compare<item>             compare = inc::default_compare<item>) const {
            the.index_of_last(value, match, compare);
        }
    };
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_index_of_last::xuser
