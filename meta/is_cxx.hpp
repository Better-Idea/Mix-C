#ifndef xpack_meta_is_cxx
#define xpack_meta_is_cxx
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_is_cxx::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_is_cxx::origin{
    template<class may_cxx_t>
    concept is_cxx = may_cxx_t::im_cxx::ensure;
}

#endif

xexport_space(mixc::meta_is_cxx::origin)