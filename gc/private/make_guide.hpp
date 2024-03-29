/*
在 GC 组件中，make_guide() 函数将 routing 和 collect 两个模板桥接起来，
为 GC 提供运行时的路由指引，若根类型 root 并不存在环形引用路径，则返回空类型集合 tlist<>
*/

#ifndef xpack_gc_private_make_guide
#define xpack_gc_private_make_guide
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::gc_private_make_guide::inc
#include"define/base_type.hpp"
#include"gc/private/collect.hpp"
#include"gc/private/routing.hpp"
#include"macro/xexport.hpp"
#include"meta/is_class.hpp"
#include"meta_seq/tlist.hpp"
#pragma pop_macro("xuser")

namespace mixc::gc_private_make_guide{
    template<class root_t>
    inline auto make_guide() {
        using namespace inc;
        if constexpr (is_class<root_t>){
            using routing_result = typename routing<root_t>::result_kvlist;
            using guide = typename collect<root_t, routing_result>::result_list;
            return guide();
        }
        else{
            return tlist<>();
        }
    }
}

#endif

xexport(mixc::gc_private_make_guide::make_guide)