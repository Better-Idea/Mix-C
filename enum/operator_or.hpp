#ifndef xpack_meta_operator_or
#define xpack_meta_operator_or
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_operator_or::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"meta/is_enum_class.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_operator_or::origin{
    template<inc::is_enum_class object_t>
    inline object_t operator | (object_t left, object_t right){
        static_assert(sizeof(object_t) <= sizeof(u64));
        return object_t(static_cast<u64>(left) | static_cast<u64>(right));
    }
}

#endif

xexport_space(mixc::meta_operator_or::origin)