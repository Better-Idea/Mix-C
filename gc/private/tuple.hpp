/*
在 GC 组件中，该 tuple 模板作为 GC 内部专用类型，用于组织 GC 的路由
该模板反射对应结构体的成员变量，根据 member_list 模板参数将对应的结构体布局在该 tuple 中
示意：如下

struct foo {
    type_first  first;
    type_second second;
    ...

    using member_list = tlist<
        type_first,
        type_second,
        ...
    >;
};

tuple<foo::member_list> xx;

我们本来打算使用 C++17 的结构化绑定来实现反射，但它不能绑定是子类的结构：
    struct foo : bar{
        ...
    };

    auto [xx] = foo(); // error
*/

#ifndef xpack_gc_tuple
#define xpack_gc_tuple
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::gc_tuple
        #include"define/base_type.hpp"
        #include"gc/private/self_management.hpp"
        #include"gc/private/routing_result.hpp"
        #include"macro/xdebug.hpp"
        #include"memop/cast.hpp"
        #include"meta/is_class.hpp"
        #include"meta/is_based_on.hpp"
        #include"meta/remove_membership.hpp"
        #include"meta_seq/tlist.hpp"
        #include"meta_seq/vlist.hpp"
        #include"meta_seq/tin.hpp"
    #pragma pop_macro("xuser")

    #define xroot   ((*(root_t *)this).*first)

    namespace mixc::gc_tuple{
        using namespace inc;

        template<class root_t, class member_list> union tuple;
        template<class root_t, auto first, auto ... values>
        union tuple<root_t, vlist<first, values...>>{
        private:
            tuple<root_t, vlist<values...>> next;
        public:
            // 重点 ======================================================================================
            // 因为在 GC 路由时的状态依赖于路径，所以需要保证 routing 和 clear_footmark 的路由路径是一致的
            // 这里统一先经过 item 在经过 next
            template<class guide>
            routing_result routing(guide gui){
                using origin = typename remove_membership<decltype(first)>::result;
                routing_result                  r = { 0 };

                if constexpr (is_class<origin>){
                    if constexpr (is_based_on<self_management, origin>){
                        if constexpr (tin<guide, origin>){
                            xdebug(im_inner_gc_tuple_routing, "%s | routing\n", xtypeid(origin).name());

                            if (r = xroot.routing(gui); r.can_arrive_root) {
                                r.degree_dvalue -= 1; // 如果 item 可以到达根节点，那么该 tuple 就存在 1 条出度

                                xdebug(im_inner_gc_tuple_routing, "%s | routing io:%lld\n", 
                                    xtypeid(origin).name(), 
                                    r.degree_dvalue
                                );
                            }
                        }
                    }
                    else{
                        using member_tuple = tuple<origin, typename origin::member_list>;
                        r = cast<member_tuple>(xroot).routing(gui);
                    }
                }
                
                r += next.routing(gui);
                return r;
            }

            template<class guide>
            void clear_footmark(guide gui, voidp root){
                using origin = typename remove_membership<decltype(first)>::result;

                if constexpr (is_class<origin>){
                    if constexpr (is_based_on<self_management, origin>){
                        if constexpr (tin<guide, origin>){
                            xdebug(im_inner_gc_tuple_clear_footmark, "%s | clear\n", xtypeid(origin).name());
                            xroot.clear_footmark(gui, root);
                        }
                    }
                    else{
                        using member_tuple = tuple<root_t, typename origin::member_list>;
                        cast<member_tuple>(xroot).clear_footmark(gui, root);
                    }
                }
                next.clear_footmark(gui, root);
            }
        };

        template<class root_t>
        union tuple<root_t, vlist<>>{
            template<class guide>
            inc::routing_result routing(guide){
                return { 0 };
            }

            template<class guide>
            void clear_footmark(guide, voidp){}
        };
    }

#endif

namespace xuser::inc{
    using ::mixc::gc_tuple::tuple;
}
