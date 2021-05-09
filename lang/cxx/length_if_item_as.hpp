#ifndef xusing_lang_cxx
#include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_length_if_item_as
#define xpack_lang_cxx_length_if_item_as
#pragma push_macro("xuser")
#pragma push_macro("xusing_lang_cxx")
#undef  xusing_lang_cxx
#undef  xuser
#define xuser mixc::lang_cxx_length_if_item_as::inc
#include"define/base_type.hpp"
#include"meta/is_base_char.hpp"
#include"lang/cxx.hpp"
#include"lang/wxx/length_if_as_utf8.hpp"
#include"lang/wxx.hpp"
#pragma pop_macro("xusing_lang_cxx")
#pragma pop_macro("xuser")

namespace mixc::lang_cxx_length_if_item_as{
    template<class item_t>
    struct core {
        using the_t = inc::cxx<item_t>;

        template<inc::is_base_char char_t>
        uxx length_if_item_as() const {
            // 相同类型的串之间转换以及宽字符串之间的转换
            if constexpr (
                (sizeof(char_t) == sizeof(item_t)) or 
                (sizeof(char_t) >= 2 and sizeof(item_t) >= 2)
            ){
                return the.length();
            }

            // 宽字符串转多字节串
            else if constexpr (sizeof(char_t) == 1){
                uxx i;
                uxx length = 0;
                for(i = 0; i < the.length();){
                    length += inc::wxx<item_t>{ the[i] }.length_if_as_utf8();
                    i += 1;
                }
                return length;
            }

            // 多字节串转宽字符串
            else{
                uxx i;
                uxx length = 0;
                for(i = 0; i < the.length();){
                    i += inc::wxx<item_t>{ the[i] }.length_if_as_utf8();
                    length += 1;
                }
                return length;
            }
        }
    };

    template<class final_t, class base_t, class item_t>
    struct meta : base_t {
        using base_t::base_t;
        using the_t         = core<item_t>;

        template<inc::is_base_char char_t>
        uxx length_if_item_as() const {
            return the.template length_if_item_as<char_t>();
        }

        template<inc::is_base_char char_t>
        final_t & length_if_item_as(uxx * value) const {
            *value = the.template length_if_item_as<char_t>();
            return thex;
        }
    };
}

#endif

namespace mixc::lang_cxx_length_if_item_as::xuser {
    template<class final_t, class item_t>
    using cxx = meta<final_t, xusing_lang_cxx::cxx<final_t, item_t>, item_t>;
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_length_if_item_as::xuser
