#ifndef xusing_lang_cxx
#include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_private_parse_base64x
#define xpack_lang_cxx_private_parse_base64x
#pragma push_macro("xuser")
#pragma push_macro("xusing_lang_cxx")
#undef  xusing_lang_cxx
#undef  xuser
#define xuser mixc::lang_cxx_private_parse_base64x::inc
#include"define/base_type.hpp"
#include"interface/can_alloc_seq.hpp"
#include"interface/seqptr.hpp"
#include"interface/unified_seq.hpp"
#include"lang/cxx/private/base64.hpp"
#include"lang/cxx/private/parse_xxx_result.hpp"
#include"lang/cxx.hpp"
#include"macro/xexport.hpp"
#include"meta/remove_ref.hpp"
#pragma pop_macro("xusing_lang_cxx")
#pragma pop_macro("xuser")

namespace mixc::lang_cxx_private_parse_base64x::origin{
    template<class seq_t>
    using parse_base64x_result  = inc::parse_xxx_result<seq_t>;

    using namespace inc::lookup;
}

namespace mixc::lang_cxx_private_parse_base64x{
    template<class item_t>
    struct core {
        using the_t = inc::cxx<item_t>;

        template<class alloc_t>
        auto parse_base64x(cu08p lut, alloc_t const & alloc) const {
            using seq_t         = decltype(alloc(0));
            using result_t      = origin::parse_base64x_result<seq_t>;
            using buffer_item_t = inc::remove_ref<decltype(alloc(0)[0])>;
            auto skip           = uxx(0);
            auto len            = the.length();

            while(len-- > 0){
                if (the[len] != '='){
                    break;
                }
                else{
                    skip   += 1;
                }
            }

            // 末尾不应该存在超过 3 个 '=' 等于号
            if (skip > 3){
                return result_t(len);
            }

            auto origin_str     = the.shorten(skip);
            auto real_length    = origin_str.length();
            auto width          = real_length * 3;
            auto size           = width / 4;
            auto total_length   = size + width % 4;
            auto buffer         = alloc(total_length);
            auto i              = uxx(0);
            auto j              = uxx(0);
            auto temp           = u32(0);

            for(; i + 4 <= real_length; i += 4){
                for(uxx j = 0; j < 4; j++){
                    if (auto bits = lut[u08(the[i + j])]; bits == 0xff){
                        return result_t(i + j);
                    }
                    else{
                        temp    = temp << 6 | bits;
                    }
                }

                // buffer_item_t 不一定是 1 个字节，也不一定是无符号数
                buffer[j++]     = buffer_item_t(u08(temp >> 16));
                buffer[j++]     = buffer_item_t(u08(temp >>  8));
                buffer[j++]     = buffer_item_t(u08(temp >>  0));
            }

            if (uxx shift = 32 - 6, rest = real_length - i; rest != 0){
                for(temp = 0; i < real_length; i++){
                    if (auto bits = lut[u08(the[i])]; bits == 0xff){
                        return result_t(i);
                    }
                    else{
                        temp   |= u32(bits) << shift;
                        shift  -= 6;
                    }
                }

                for(i = 0; i < rest; i++){
                    buffer[j++] = buffer_item_t(u08(temp >> 24));
                    temp      <<= 8;
                }
            }
            return result_t(buffer);
        }
    };

    template<class final_t, class base_t, class item_t>
    struct meta : base_t{
        using base_t::base_t;
        using the_t = core<item_t>;

        template<inc::can_alloc_seq alloc_t>
        auto parse(inc::base64_t mode, alloc_t const & alloc) const {
            using ret_t     = decltype(alloc(0));
            using seq_t     = inc::unified_seq<ret_t>;
            auto allocx     = inc::ialloc_seq<seq_t>(alloc);
            return the.parse_base64x(mode, allocx);
        }
    };
}

#endif

namespace mixc::lang_cxx_private_parse_base64x::xuser{
    template<class final_t, class item_t>
    using cxx = meta<final_t, xusing_lang_cxx::cxx<final_t, item_t>, item_t>;
}

xexport_space(mixc::lang_cxx_private_parse_base64x::origin)

#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_private_parse_base64x::xuser
