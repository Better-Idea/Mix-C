#ifndef xpack_meta_operator_inversion
#define xpack_meta_operator_inversion
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_operator_inversion::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"meta/is_enum_class.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_operator_inversion::origin{
    template<inc::is_enum_class object_t>
    inline object_t operator ~ (object_t left){
        static_assert(sizeof(object_t) <= sizeof(u64));
        return object_t(~static_cast<u64>(left));
    }
}

#endif

xexport_space(mixc::meta_operator_inversion::origin)