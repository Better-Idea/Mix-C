#ifndef xpack_meta_has_constructor
#define xpack_meta_has_constructor
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_has_constructor::inc
#include"define/base_type.hpp"
#include"macro/xnew.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_has_constructor::origin{
    template<class object_t, class ... args>
    concept has_constructor = requires(object_t * ptr, args ... list){
        xnew(ptr) object_t(list...);
    };
}

#endif

xexport_space(mixc::meta_has_constructor::origin)