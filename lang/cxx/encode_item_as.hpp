#ifndef xusing_lang_cxx
#include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_encode_item_as
#define xpack_lang_cxx_encode_item_as
#pragma push_macro("xuser")
#pragma push_macro("xusing_lang_cxx")
#undef  xusing_lang_cxx
#undef  xuser
#define xuser mixc::lang_cxx_encode_item_as::inc
#include"define/base_type.hpp"
#include"interface/can_alloc.hpp"
#include"lang/cxx/length_if_item_as.hpp"
#include"lang/cxx/encode_first_as.hpp"
#include"lang/cxx.hpp"
#include"lang/wxx/length_if_as_utf8.hpp"
#include"lang/wxx.hpp"
#include"macro/xstruct.hpp"
#include"memop/copy.hpp"
#include"meta/is_base_char.hpp"
#include"meta/unsigned_type.hpp"
#include"meta/is_same.hpp"
#pragma pop_macro("xusing_lang_cxx")
#pragma pop_macro("xuser")

namespace mixc::lang_cxx_encode_item_as{
    template<class item_t>
    struct core {
        using the_t = inc::cxx<item_t>;

        template<inc::is_base_char char_t, class alloc_t>
        inc::cxx<char_t> encode_item_as(alloc_t const & alloc) const {
            typedef struct{
                void operator=(item_t value){
                    // 窄化时作饱和运算
                    if constexpr (sizeof(char_t) < sizeof(item_t)){
                        auto max_value_of_item = (1 << (sizeof(char_t) * 8)) - 1;
                        item = value > max_value_of_item ? char_t(-1) : value;
                    }
                    else{
                        item = value;
                    }
                }

                char_t item;
            } * saturate_wrapp;

            // 若是同类型串或者都是宽字符串
            if constexpr (
                (inc::is_same<char_t, item_t>) or
                (sizeof(item_t) >= 2 and sizeof(char_t) >= 2)
            ){
                auto length = the.length();
                auto target = alloc(length);
                inc::copy(saturate_wrapp(target), (item_t *)the, length);
                return inc::cxx<char_t>{ target, length };
            }

            // 多字节串、宽字符串互转
            else{
                auto source = the;
                auto source_length = the.length();
                auto target_length = the.template length_if_item_as<char_t>();
                auto target = alloc(target_length);
                auto ptr = target;

                for(uxx i = 0; i < source_length;){
                    auto sequence = source.template encode_first_as<char_t>();
                    inc::copy(ptr, sequence.ptr(), sequence.length());
                    source = source.backward(sequence.length());
                    i += sequence.skip_to_next();
                    ptr += sequence.length();
                }
                
                return inc::cxx<char_t>{ target, target_length };
            }
        }
    };

    template<class final_t, class base_t, class item_t>
    struct meta : base_t{
        using base_t::base_t;
        using the_t = core<item_t>;
        
        template<inc::is_base_char char_t, class alloc_t>
        requires(
            inc::can_alloc<alloc_t, char_t>
        )
        inc::cxx<char_t> encode_item_as(alloc_t const & alloc) const {
            return the.template encode_item_as<char_t>(alloc);
        }
    };
}

#endif

namespace mixc::lang_cxx_encode_item_as::xuser{
    template<class final_t, class item_t>
    using cxx = meta<final_t, xusing_lang_cxx::cxx<final_t, item_t>, item_t>;
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_encode_item_as::xuser
