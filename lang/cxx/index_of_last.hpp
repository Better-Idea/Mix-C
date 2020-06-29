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
    #include"interface/initializer_list.hpp"
    #include"lang/cxx.hpp"
    
    namespace xuser{
        template<class item>
        struct core : inc::cxx<item> {
            using base_t = inc::cxx<item>;
            using base_t::base_t;
            using the_t = core<item>;

            core(base_t const & self) : 
                base_t(self){}

            uxx index_of_last(item pattern, inc::can_compare<item> compare) const {
                return index_of_last(& pattern, 1, compare);
            }

            uxx index_of_last(item const * pattern, uxx length, inc::can_compare<item> compare) const {
                for(uxx i = the.length(); i--; ){
                    for(uxx ii = 0; ii < length; ii++){
                        if (compare(the[i], pattern[ii]) == 0){
                            return i;
                        }
                    }
                }
                return not_exist;
            }
            

            template<class compare_invoke>
            uxx index_of_last(
                the_t                  pattern,
                compare_invoke const & compare) const {

                // miss 为未匹配项的索引
                // 每次都去匹配上一次未匹配的字符 str[miss]
                // 每当在 index 处匹配，就将原串缩短 index - miss + 1
                //      剩余可匹配的串
                //            |
                //         |--+--|
                // origin "123451235"
                // pattern        "34"
                the_t origin = the;
                the_t temp   = the;
                uxx   miss   = 0;
                uxx   index;

                if (origin.length() < pattern.length() or pattern.length() == 0) {
                    return not_exist;
                }
                for (origin = origin.shorten(pattern.length() - 1);;){
                    if (index = origin.index_of_last(pattern[miss], compare); index == not_exist){
                        break;
                    }

                    origin.length(index - miss);
                    temp          = origin.backward(origin.length());
                    origin        = origin.elongate(1);
                    
                    for (index = 0; ; index++){
                        if (index == pattern.length()) {
                            return origin.length() - 1;
                        }
                        if (compare(temp[index], pattern[index]) != 0) {
                            miss = index;
                            break;
                        }
                    }
                }
                return not_exist;
            }

            void index_of_last(
                the_t                              pattern, 
                inc::can_callback<void(uxx index)> match,
                inc::can_compare<item>             compare) const {

                for(auto cur = the;;){
                    if (uxx i = cur.index_of_last(pattern, compare); i == not_exist){
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

    #pragma pop_macro("xusing_lang_cxx")
    #pragma pop_macro("xuser")
#endif

namespace mixc::lang_cxx_index_of_last::xuser {
    template<class final, class item>
    struct cxx : xusing_lang_cxx::cxx<final, item> {
        using xusing_lang_cxx::cxx<final, item>::cxx;
        using the_t = core<item>;

        uxx index_of_last(
            item                   pattern, 
            inc::can_compare<item> compare = inc::default_compare<item>) const {
            return the.index_of_last(pattern, compare);
        }

        uxx index_of_last(
            inc::initializer_list<item> patterns, 
            inc::can_compare<item>      compare = inc::default_compare<item>) const {
            return the.index_of_last(patterns.begin(), patterns.size(), compare);
        }

        uxx index_of_last(
            final                  pattern, 
            inc::can_compare<item> compare = inc::default_compare<item>) const {
            return the.index_of_last(pattern, compare);
        }

        final & index_of_last(
            final                              pattern, 
            inc::can_callback<void(uxx index)> match,
            inc::can_compare<item>             compare = inc::default_compare<item>) const {
            the.index_of_last(pattern, match, compare);
            return thex;
        }
    };
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_index_of_last::xuser
