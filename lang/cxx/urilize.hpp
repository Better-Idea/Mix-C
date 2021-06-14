#ifndef xusing_lang_cxx
#include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_urilize
#define xpack_lang_cxx_urilize
#pragma push_macro("xuser")
#pragma push_macro("xusing_lang_cxx")
#undef  xusing_lang_cxx
#undef  xuser
#define xuser mixc::lang_cxx_urilize::inc
#include"define/base_type.hpp"
#include"interface/can_alloc.hpp"
#include"interface/unified_seq.hpp"
#include"lang/cxx/private/lut.uri.hpp"
#include"lang/cxx/encode_first_as.hpp"
#include"lang/cxx.hpp"
#include"lang/wxx/length_if_as_utf8.hpp"
#include"lang/wxx.hpp"
#include"macro/xref.hpp"
#include"meta/char_type.hpp"
#include"meta/unsigned_type.hpp"
#pragma pop_macro("xusing_lang_cxx")
#pragma pop_macro("xuser")

namespace mixc::lang_cxx_urilize::lookup{
    using namespace inc::lookup;
}

namespace mixc::lang_cxx_urilize{
    template<class item_t>
    struct core {
        using the_t = inc::cxx<item_t>;

        template<class seq_t, class alloc_t>
        auto urilize(seq_t const & seq, inc::uri_t const & need_convert, alloc_t const & alloc){
            using char_t            = inc::char_type<decltype(seq[0])>;
            using unsigned_t        = inc::unsigned_type<char_t>;
            auto total_length       = uxx(0);

            for(uxx i = 0, len = seq.length(); i < len; i++){
                if (auto item = unsigned_t(seq[i]); sizeof(item) > 1 and item > 255){
                    total_length   += 3 * inc::wxx<char_t>{ char_t(seq[i]) }.length_if_as_utf8();
                }
                else if (item >= 128 or need_convert[item]){
                    total_length   += 3;    // %xx 三个字符
                }
                else{
                    total_length   += 1;
                }
            }

            auto buffer             = alloc(total_length);
            auto hex_lut            = "0123456789ABCDEF";
            auto encode             = [&](auto buffer, auto * i_ptr, auto source, auto length){
                for(uxx i = 0, & j  = *i_ptr; i < length; i++){
                    auto item       = u08(source[i]);
                    buffer[j++]     = '%';
                    buffer[j++]     = hex_lut[item >> 4];
                    buffer[j++]     = hex_lut[item & 0xf];
                }
            };
            
            for(uxx i = 0, j = 0, len = seq.length(); i < len; i++){
                if (auto item = unsigned_t(seq[i]); sizeof(item) > 1 and item > 255){
                    auto k          = uxx(0);
                    auto c          = char_t(seq[i]);
                    auto one        = inc::cxx<char_t>{ xref(c), 1 };
                    auto utf8       = one.template encode_first_as<char>();
                    encode(buffer, xref(j), utf8, utf8.length());
                }
                else if (item >= 128 or need_convert[item]){
                    encode(buffer, xref(j), xref(item), 1);
                }
                else{
                    buffer[j++]     = item;
                }
            }

            the                     = the_t{ buffer, total_length };
        }
    };

    template<class final_t, class base_t, class item_t>
    struct meta : base_t{
        using base_t::base_t;
        using the_t = core<item_t>;

        template<class seq_t, class alloc_t>
        requires(
            inc::can_unified_seqlize<seq_t> and
            inc::can_alloc<alloc_t, item_t>
        )
        meta(seq_t const & seq, inc::uri_t const & mode, alloc_t const & alloc){
            auto source     = inc::unified_seq<seq_t>(seq);
            the.urilize(source, mode, alloc);
        }
    };
}

#endif

namespace mixc::lang_cxx_urilize::xuser{
    template<class final_t, class item_t>
    using cxx = meta<final_t, xusing_lang_cxx::cxx<final_t, item_t>, item_t>;
}

xexport_space(mixc::lang_cxx_urilize::lookup)

#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_urilize::xuser
