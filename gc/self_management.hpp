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
    #define xuser mixc::gc_self_management
    #include"define/base_type.hpp"
    #include"macro/xgc.hpp"

    namespace xuser::origin{
        struct self_management{
            xgc_fields(
                xiam(self_management)
            );
        };
    }
    #pragma pop_macro("xuser")
#endif

namespace xuser::inc{
    using namespace ::mixc::gc_self_management::origin;
}
