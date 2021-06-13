#ifndef xusing_lang_cxx
#include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_parse_base64uri
#define xpack_lang_cxx_parse_base64uri
#pragma push_macro("xuser")
#pragma push_macro("xusing_lang_cxx")
#undef  xusing_lang_cxx
#undef  xuser
#define xuser mixc::lang_cxx_parse_base64uri::inc
#include"define/base_type.hpp"
#include"interface/can_alloc_seq.hpp"
#include"lang/cxx/private/parse_base64x.hpp"
#include"lang/cxx.hpp"
#pragma pop_macro("xusing_lang_cxx")
#pragma pop_macro("xuser")

namespace mixc::lang_cxx_parse_base64uri::origin{
    template<class seq_t>
    using parse_base64uri_result  = inc::parse_base64x_result<seq_t>;
}

namespace mixc::lang_cxx_parse_base64uri{
    template<class final_t, class base_t, class item_t>
    struct meta : base_t {
        using base_t::base_t;
        using the_t = inc::cxx<item_t>;

        template<inc::can_alloc_seq alloc_t>
        auto parse_base64uri(alloc_t const & alloc) const {
            return the.parse(inc::base64uri, alloc);
        }
    };
}

#endif

namespace mixc::lang_cxx_parse_base64uri::xuser {
    template<class final_t, class item_t>
    using cxx = meta<final_t, xusing_lang_cxx::cxx<final_t, item_t>, item_t>;
}

xexport_space(mixc::lang_cxx_parse_base64uri::origin)

#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_parse_base64uri::xuser
