/*
在 GC 组件中，make_guide() 函数将 routing 和 collect 两个模板桥接起来，
为 GC 提供运行时的路由指引，若根类型 root 并不存在环形引用路径，则返回空类型集合 tlist<>
*/

#ifndef xpack_gc_make_guide
#define xpack_gc_make_guide
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::gc_make_guide
        #include"define/base_type.hpp"
        #include"gc/private/routing.hpp"
        #include"gc/private/collect.hpp"
        #include"meta/is_class.hpp"
        #include"meta_seq/tlist.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::gc_make_guide{
        template<class root>
        inline auto make_guide() {
            using namespace inc;
            if constexpr(is_class<root>){
                using routing_result = typename routing<root>::result_kvlist;
                using guide = typename collect<root, routing_result>::result_list;
                return guide();
            }
            else{
                return tlist<>();
            }
        }
    }

#endif

namespace xuser::inc{
    using ::mixc::gc_make_guide::make_guide;
}
