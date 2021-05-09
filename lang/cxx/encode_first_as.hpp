#ifndef xusing_lang_cxx
#include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_encode_first_as
#define xpack_lang_cxx_encode_first_as
#pragma push_macro("xuser")
#pragma push_macro("xusing_lang_cxx")
#undef  xusing_lang_cxx
#undef  xuser
#define xuser mixc::lang_cxx_encode_first_as::inc
#include"define/base_type.hpp"
#include"interface/can_alloc.hpp"
#include"lang/cxx.hpp"
#include"lang/wxx/length_if_as_utf8.hpp"
#include"lang/wxx.hpp"
#include"macro/xstruct.hpp"
#include"meta/is_base_char.hpp"
#include"meta/unsigned_type.hpp"
#include"meta/is_same.hpp"
#pragma pop_macro("xusing_lang_cxx")
#pragma pop_macro("xuser")

namespace mixc::lang_cxx_encode_first_as{
    enum encode_first_as_result_t{
        success,
        char_seqence_missing,
        overflow,
        narrow,
    };

    template<inc::is_base_char item_t>
    xstruct(
        xtmpl(encode_first_as_result, item_t)
    )
        encode_first_as_result(encode_first_as_result_t value, uxx skip_to_next) :
            c08{0},
            plength(0),
            pskip_to_next(skip_to_next),
            pstate(u08(value)){
        }

        encode_first_as_result(item_t const * value, uxx length, uxx skip_to_next) :
            c08{0},
            plength(length),
            pskip_to_next(skip_to_next),
            pstate(u08(encode_first_as_result_t::success)){

            for(uxx i = 0; i < length; i++){
                the[i] = value[i];
            }
        }

        operator item_t &(){
            return the[0];
        }

        item_t * ptr(){
            return (item_t *)the.c08;
        }

        item_t & operator[](uxx index){
            return ((item_t *)the.c08)[index];
        }

        using final_t = the_t;

        xpubgetx(length, uxx){
            return plength;
        }

        xpubgetx(skip_to_next, uxx){
            return pskip_to_next;
        }

        xpubgetx(is_success, bool){
            return pstate == encode_first_as_result_t::success;
        }

        xpubgetx(is_char_seqence_missing, bool){
            return pstate == encode_first_as_result_t::char_seqence_missing;
        }

        xpubgetx(is_overflow, bool){
            return pstate == encode_first_as_result_t::overflow;
        }

        xpubgetx(is_narrow, bool){
            return pstate == encode_first_as_result_t::narrow;
        }

    private:
        union{
            char        c08[7];
            char16_t    c16;
            char32_t    c32;
        };

        u08 plength         : 3;
        u08 pskip_to_next   : 3;
        u08 pstate          : 2;
    $

    template<class item_t>
    struct core {
        using the_t = inc::cxx<item_t>;

        template<inc::is_base_char char_t>
        encode_first_as_result<char_t> encode_first_as() const {
            // 从 byte1 开始每个字节只有 6bit 是有效的
            // bytes    range                   byte0    byte1    byte2    byte3    byte4    byte5
            // 1        0000 0000 - 0000 007F   0xxxxxxx
            // 2        0000 0080 - 0000 07FF   110xxxxx 10xxxxxx
            // 3        0000 0800 - 0000 FFFF   1110xxxx 10xxxxxx 10xxxxxx
            // 4        0001 0000 - 0010 FFFF   11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
            // 5        0020 0000 - 03FF FFFF   111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
            // 6        0400 0000 - 7FFF FFFF   1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 
            auto        word   = inc::unsigned_type<item_t>(the[0]);
            char32_t    w32    = 0;
            uxx         length = 1;

            if constexpr (inc::is_same<item_t, char>){
                // length 表示将当前字符编码成 utf8 字符所需的字节数
                if (length = inc::wxx<item_t>{ the[0] }.length_if_as_utf8();
                    length > the.length()){
                    return { encode_first_as_result_t::char_seqence_missing, length };
                }

                if constexpr (sizeof(char_t) == 1){
                    return { & the[0], length, length };
                }

                if (length == 1){
                    w32 = char32_t(word);
                }
                else{
                    u08 rest_bits = (length - 1) * 6/*utf-8 剩余的每个字节实际有效位数*/;
                    u08 first_bits = (7 - length);
                    u08 mask = (1 << first_bits) - 1;
                    w32 = char32_t(u08(word) & mask) << rest_bits;

                    for(uxx i = 1; i < length; i++){
                        rest_bits -= 6;
                        w32 |= char32_t(u08(the[i] & 0x3f)) << rest_bits;
                    }
                }

                if constexpr (sizeof(char_t) == 2){
                    if (w32 > 0xffff){
                        return { encode_first_as_result_t::overflow, length };
                    }

                    char16_t value = char16_t(w32);
                    return { & value, 1, length };
                }

                if constexpr (sizeof(char_t) == 4){
                    return { & w32, 1, length };
                }
            }
            // 宽字节字符转单字节字符
            else if constexpr (inc::is_same<char_t, char> and sizeof(item_t) >= 2){
                char buffer[8] = {0};
                length = inc::wxx<item_t>{ word }.length_if_as_utf8();
                
                if (length == 1){
                    buffer[0] = char(word);
                    return { buffer, 1, 1 };
                }

                u08 rest_bits = u08(length - 1) * 6;
                u08 first_bits = u08(7 - length);
                u08 prefix = u08(-2 << first_bits);
                buffer[0] = prefix | (word >> rest_bits);

                for(uxx i = length; i-- > 1;){
                    buffer[i] = (word & 0x3f) | 0x80;
                    word >>= 6;
                }

                return { buffer, length, 1 };
            }
            // 宽字符间转换
            else {
                // 宽度一致
                if constexpr (inc::is_same<item_t, char_t>){
                    auto value = char_t(word);
                    return { & value, 1, 1 };
                }
                // 宽化
                else if constexpr (inc::is_same<item_t, char16_t>){
                    auto value = char32_t(word);
                    return { & value, 1, 1 };
                }
                // 窄化
                else if (word > 0xffff){
                    return { encode_first_as_result_t::narrow, 1 };
                }
                else{
                    auto value = char16_t(word);
                    return { & value, 1, 1 };
                }
            }
        }
    };

    template<class final_t, class base_t, class item_t>
    struct meta : base_t{
        using base_t::base_t;
        using the_t = core<item_t>;
        
        template<inc::is_base_char char_t>
        encode_first_as_result<char_t> encode_first_as() const {
            return the.template encode_first_as<char_t>();
        }
    };
}

#endif

namespace mixc::lang_cxx_encode_first_as::xuser{
    using ::mixc::lang_cxx_encode_first_as::encode_first_as_result;

    template<class final_t, class item_t>
    using cxx = meta<final_t, xusing_lang_cxx::cxx<final_t, item_t>, item_t>;
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_encode_first_as::xuser
