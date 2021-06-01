/*
在 GC 组件中，self_management用于判定一个类是否带有 GC 成分
判定方法 is_based_on：
    我们定义：只要是继承了 self_management 的类都应该具有以下私有方法
        - template<class guide> bool routing<guide>();
*/

#ifndef xpack_gc_self_management
#define xpack_gc_self_management
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::gc_self_management::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"macro/xstruct.hpp"
#pragma pop_macro("xuser")

namespace mixc::gc_self_management{
    xstruct(
        xname(self_management)
    ) $
}

#endif

xexport(mixc::gc_self_management::self_management)