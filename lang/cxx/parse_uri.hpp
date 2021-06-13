#ifndef xusing_lang_cxx
#include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_parse_uri
#define xpack_lang_cxx_parse_uri
#pragma push_macro("xuser")
#pragma push_macro("xusing_lang_cxx")
#undef  xusing_lang_cxx
#undef  xuser
#define xuser mixc::lang_cxx_parse_uri::inc
#include"define/base_type.hpp"
#include"interface/can_alloc_seq.hpp"
#include"interface/unified_seq.hpp"
#include"lang/cxx/private/lut.uri.hpp"
#include"lang/cxx/private/parse_xxx_result.hpp"
#include"lang/cxx/encode_first_as.hpp"
#include"lang/cxx.hpp"
#include"lang/wxx/is_hex.hpp"
#include"lang/wxx/length_if_as_utf8.hpp"
#include"lang/wxx/parse_hex.hpp"
#include"lang/wxx.hpp"
#include"macro/xexport.hpp"
#include"meta/remove_ref.hpp"
#include"meta/unsigned_type.hpp"
#pragma pop_macro("xusing_lang_cxx")
#pragma pop_macro("xuser")

namespace mixc::lang_cxx_parse_uri::origin{
    template<class seq_t>
    using parse_uri_result = inc::parse_xxx_result<seq_t>;

    using namespace inc::lookup;
}

namespace mixc::lang_cxx_parse_uri{
    template<class item_t>
    struct core {
        using the_t = inc::cxx<item_t>;

        template<class alloc_t>
        auto parse_uri(inc::uri_t const & need_convert, alloc_t const & alloc){
            using unsigned_t        = inc::unsigned_type<item_t>;
            using seq_t             = decltype(alloc(0));
            using result_t          = origin::parse_uri_result<seq_t>;
            using buffer_item_t     = inc::remove_ref<decltype(alloc(0)[0])>;
            auto total_length       = uxx(0);
            auto i                  = uxx(0);
            auto len                = the.length();

            for(; i + 2 < len; total_length++, i++){
                if (auto c = unsigned_t(the[i]); c != '%'){
                    if (c >= 128 or need_convert[c]){
                        return result_t{ i };
                    }
                    continue;
                }
                if (not inc::wxx<item_t>{ the[++i] }.is_hex() or 
                    not inc::wxx<item_t>{ the[++i] }.is_hex()){
                    return result_t{ i };
                }
            }

            for(total_length += len - i; i < len; i++){
                if (auto c = unsigned_t(the[i]); c >= 128 or need_convert[c]){
                    return result_t{ i };
                }
            }

            auto buffer             = alloc(total_length);
            auto j                  = uxx(0);
            
            for(i = 0; i < len;){
                if (auto c = u08(the[i++]); c != '%'){
                    buffer[j++]     = item_t(c);
                    continue;
                }

                auto h              = u08(inc::w08{ char(the[i++]) }.parse_hex());
                auto l              = u08(inc::w08{ char(the[i++]) }.parse_hex());
                auto first          = char(h << 4 | l);

                if (sizeof(item_t) == 1 or u08(first) < 128){
                    buffer[j++]     = item_t(first);
                    continue;
                }
                
                char items[8]       = {};
                auto count          = inc::w08{ first }.length_if_as_utf8();
                auto need           = uxx(count - 1/*first*/) * 3;
                auto k              = uxx(1);
                auto i_old          = i;
                auto end            = i + need;

                for(items[0] = first; i + 3 <= end; i += 3){
                    auto token      = char(the[i + 0]);
                    auto wh         = char(the[i + 1]);
                    auto wl         = char(the[i + 2]);

                    if (token != '%'){
                        return result_t{ i + 0 };
                    }
                    if (not inc::w08{ wh }.is_hex()){
                        return result_t{ i + 1 };
                    }
                    if (not inc::w08{ wl }.is_hex()){
                        return result_t{ i + 2 };
                    }

                    auto h          = inc::w08{ wh }.parse_hex();
                    auto l          = inc::w08{ wl }.parse_hex();
                    items[k++]      = char(h << 4 | l);
                }

                if (auto item = inc::c08{ items, k }.encode_first_as<item_t>(); item.is_success()){
                    buffer[j++]     = item;
                }
                else{
                    return result_t{ i_old };
                }
            }
            return result_t { buffer };
        }
    };

    template<class final_t, class base_t, class item_t>
    struct meta : base_t{
        using base_t::base_t;
        using the_t = core<item_t>;

        template<inc::can_alloc_seq alloc_t>
        auto parse_uri(alloc_t const & alloc){
            using ret_t             = decltype(alloc(0));
            using seq_t             = inc::unified_seq<ret_t>;
            auto allocx             = inc::ialloc_seq<seq_t>(alloc);
            return the.parse_uri(origin::uri, allocx);
        }
    };
}

#endif

namespace mixc::lang_cxx_parse_uri::xuser{
    template<class final_t, class item_t>
    using cxx = meta<final_t, xusing_lang_cxx::cxx<final_t, item_t>, item_t>;
}

xexport_space(mixc::lang_cxx_parse_uri::origin)

#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_parse_uri::xuser
