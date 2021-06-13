#ifndef xpack_lang_cxx_private_parse_xxx_result
#define xpack_lang_cxx_private_parse_xxx_result
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::lang_cxx_private_parse_xxx_result::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"meta/item_origin_of.hpp"
#include"lang/cxx.hpp"
#pragma pop_macro("xuser")

namespace mixc::lang_cxx_private_parse_xxx_result::origin{
    template<class seq_t>
    struct parse_xxx_result : seq_t{
        using item_t = inc::item_origin_of<seq_t>;

        parse_xxx_result(seq_t const & self):
            seq_t(self), m_index_of_error(not_exist){
        }

        parse_xxx_result(uxx index_of_error) : 
            seq_t(inc::cxx<item_t>{}), m_index_of_error(index_of_error){
        }

        bool is_parse_error() const {
            return m_index_of_error != not_exist;
        }

        uxx index_of_error() const {
            return m_index_of_error;
        }
    private:
        uxx m_index_of_error;
    };
}

#endif

xexport_space(mixc::lang_cxx_private_parse_xxx_result::origin)
