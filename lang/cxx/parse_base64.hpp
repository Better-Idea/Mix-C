#ifndef xusing_lang_cxx
#include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_parse_base64
#define xpack_lang_cxx_parse_base64
#pragma push_macro("xuser")
#pragma push_macro("xusing_lang_cxx")
#undef  xusing_lang_cxx
#undef  xuser
#define xuser mixc::lang_cxx_parse_base64::inc
#include"define/base_type.hpp"
#include"interface/can_alloc_seq.hpp"
#include"lang/cxx/private/parse_base64x.hpp"
#include"lang/cxx.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xusing_lang_cxx")
#pragma pop_macro("xuser")

namespace mixc::lang_cxx_parse_base64::origin{
    template<class seq_t>
    using parse_base64_result  = inc::parse_base64x_result<seq_t>;
}

namespace mixc::lang_cxx_parse_base64{
    template<class final_t, class base_t, class item_t>
    struct meta : base_t {
        using base_t::base_t;
        using the_t = inc::cxx<item_t>;

        template<inc::can_alloc_seq alloc_t>
        auto parse_base64(alloc_t const & alloc) const {
            return the.parse(inc::base64, alloc);
        }
    };
}

#endif

namespace mixc::lang_cxx_parse_base64::xuser {
    template<class final_t, class item_t>
    using cxx = meta<final_t, xusing_lang_cxx::cxx<final_t, item_t>, item_t>;
}

xexport_space(mixc::lang_cxx_parse_base64::origin)

#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_parse_base64::xuser
