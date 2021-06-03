#ifndef xusing_lang_wxx
#include"lang/private/wxx.hpp"
#endif

#ifndef xpack_lang_wxx_length_if_as_utf8
#define xpack_lang_wxx_length_if_as_utf8
#pragma push_macro("xuser")
#pragma push_macro("xusing_lang_wxx")
#undef  xusing_lang_wxx
#undef  xuser
#define xuser mixc::lang_wxx_length_if_as_utf8::inc
#include"define/base_type.hpp"
#include"instruction/index_of_last_reset.hpp"
#include"lang/wxx.hpp"
#include"meta/unsigned_type.hpp"
#pragma pop_macro("xusing_lang_wxx")
#pragma pop_macro("xuser")

namespace mixc::lang_wxx_length_if_as_utf8{
    template<class type>
    struct core : inc::wxx<type>{
        using inc::wxx<type>::wxx;
        using the_t = core<type>;

        uxx length_if_as_utf8() const {
            // 无符号类型 char 可能为有符号类型（平台差异）
            // 我们这里要求 char * 配置成 utf8 字符串 ====================================
            auto u = inc::unsigned_type<type>(the.m_value);

            // ascii 码只有一个字节
            if (u < 0x80){
                return 1;
            }

            // bytes    range                   byte0    byte1    byte2    byte3    byte4    byte5
            // 1        0000 0000 - 0000 007F   0xxxxxxx
            // 2        0000 0080 - 0000 07FF   110xxxxx 10xxxxxx
            // 3        0000 0800 - 0000 FFFF   1110xxxx 10xxxxxx 10xxxxxx
            // 4        0001 0000 - 0010 FFFF   11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
            // 5        0020 0000 - 03FF FFFF   111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
            // 6        0400 0000 - 7FFF FFFF   1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 
            if constexpr (sizeof(u) == sizeof(char)){
                return 7 - inc::index_of_last_reset(u);
            }
            else if constexpr (sizeof(u) == sizeof(char16_t)){
                return u < 0x800 ? 2 : 3;
            }
            else if constexpr (sizeof(u) == sizeof(char32_t)){
                return
                    u < 0x00000800 ? 2 : 
                    u < 0x00010000 ? 3 :
                    u < 0x00110000 ? 4 : 
                    u < 0x04000000 ? 5 : 6;
            }
        }
    };

    template<class final_t, class base, class type>
    struct meta: base {
        using base::base;
        using the_t = core<type>;

        uxx length_if_as_utf8() const {
            return the.length_if_as_utf8();
        }

        final_t & length_if_as_utf8(uxx * result) const {
            result[0] = the.length_if_as_utf8();
            return thex;
        }
    };
}

#endif

namespace mixc::lang_wxx_length_if_as_utf8::xuser {
    template<class final_t, class item_t>
    using wxx = meta<final_t, xusing_lang_wxx::wxx<final_t, item_t>, item_t>;
}

#undef  xusing_lang_wxx
#define xusing_lang_wxx ::mixc::lang_wxx_length_if_as_utf8::xuser
