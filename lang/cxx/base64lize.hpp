#ifndef xusing_lang_cxx
#include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_base64lize
#define xpack_lang_cxx_base64lize
#pragma push_macro("xuser")
#pragma push_macro("xusing_lang_cxx")
#undef  xusing_lang_cxx
#undef  xuser
#define xuser mixc::lang_cxx_base64lize::inc
#include"define/base_type.hpp"
#include"interface/can_alloc.hpp"
#include"interface/unified_seq.hpp"
#include"lang/cxx/private/lut.base64x.hpp"
#include"lang/cxx.hpp"
#include"meta/item_origin_of.hpp"
#pragma pop_macro("xusing_lang_cxx")
#pragma pop_macro("xuser")

namespace mixc::lang_cxx_base64lize::lookup{
    using namespace inc::lookup;
}

namespace mixc::lang_cxx_base64lize{
    template<class item_t>
    struct core {
        using the_t = inc::cxx<item_t>;

        template<class seq_t, class alloc_t>
        auto base64lize(seq_t const & seq, asciis lut, alloc_t const & alloc){
            auto width          = seq.length() * 4;
            auto size           = width / 3;
            auto total_length   = ~0x3 & (size + 3);
            auto buffer         = alloc(total_length);
            auto temp           = u32(0);
            auto len            = seq.length();
            auto i              = uxx(0);
            auto j              = uxx(0);
            auto k              = uxx(0);
            auto l              = uxx(0);

            while(i + 3 <= len){
                temp            = temp << 8 | u08(seq[i++]);
                temp            = temp << 8 | u08(seq[i++]);
                temp            = temp << 8 | u08(seq[i++]);
                buffer[j++]     = lut[(temp >> 18) & 0x3f];
                buffer[j++]     = lut[(temp >> 12) & 0x3f];
                buffer[j++]     = lut[(temp >> 06) & 0x3f];
                buffer[j++]     = lut[(temp >> 00) & 0x3f];
            }

            if (i != len){
                for(temp = 0, k = i; i < len; ){
                    temp        = temp << 8 | u08(seq[i++]);
                }

                for(temp <<= (4 - (i - k)) * 8, l = j + i - k + 1; j < l; ){
                    buffer[j++] = lut[temp >> (32 - 6)];
                    temp      <<= 6;
                }

                if (lut != lookup::base64uri) while(j & 0x3){
                    buffer[j++] = '=';
                }
            }

            the = the_t{ buffer, j };
        }
    };

    template<class final_t, class base_t, class item_t>
    struct meta : base_t{
        using base_t::base_t;
        using the_t = core<item_t>;

        template<class seq_t, class alloc_t>
        requires(
            inc::can_unified_seqlize<seq_t> and
            inc::can_alloc<alloc_t, item_t> and
            sizeof(inc::item_origin_of<seq_t>) == 1
        )
        meta(seq_t const & seq, inc::base64_t mode, alloc_t const & alloc){
            auto source     = inc::unified_seq<seq_t>(seq);
            the.base64lize(source, mode, alloc);
        }
    };
}

#endif

namespace mixc::lang_cxx_base64lize::xuser{
    template<class final_t, class item_t>
    using cxx = meta<final_t, xusing_lang_cxx::cxx<final_t, item_t>, item_t>;
}

xexport_space(mixc::lang_cxx_base64lize::lookup)

#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_base64lize::xuser
