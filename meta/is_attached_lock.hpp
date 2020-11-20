#ifndef xpack_meta_is_attached_lock
#define xpack_meta_is_attached_lock
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_is_attached_lock::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_is_attached_lock{
    struct meta{
        void operator()(){}
    };
}

namespace mixc::meta_is_attached_lock::origin{
    template<class lock_t>
    concept is_attached_lock = requires(lock_t v, meta functor){
        // 支持传入 this 指针作为上下文
        v.template lock<0/*操作的编号*/>(& functor, functor);
    };
}

#endif

xexport_space(mixc::meta_is_attached_lock::origin)