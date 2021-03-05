#ifndef xpack_meta_is_lambda
#define xpack_meta_is_lambda
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_is_lambda::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_is_lambda{
    template<class may_lambda_t>
    concept is_lambda = & may_lambda_t::operator() != nullptr;
}

#endif

xexport(mixc::meta_is_lambda::is_lambda)