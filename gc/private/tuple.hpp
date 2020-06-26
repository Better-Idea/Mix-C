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
    #include"gc/self_management.hpp"
    #include"macro/xdebug.hpp"
    #include"memop/cast.hpp"
    #include"meta/is_class.hpp"
    #include"meta/is_based_on.hpp"
    #include"meta/remove_membership.hpp"
    #include"meta_seq/tlist.hpp"
    #include"meta_seq/vlist.hpp"
    #include"meta_seq/tin.hpp"

    // 成员变量指针的魔法
    #define xroot   ((*(root_t *)this).*first)

    namespace xuser::origin{
        using namespace inc;

        template<class root_t, class member_list> union tuple;
        template<class root_t, auto first, auto ... values>
        union tuple<root_t, vlist<first, values...>>{
        private:
            tuple<root_t, vlist<values...>> next;
            using origin = typename remove_membership<decltype(first)>::result;
        public:
            template<class guide> bool routing(){
                bool can_arrive_root = false;
                xdebug(im_gc_tuple_routing, xtypeid(origin).name);

                if constexpr (is_class<origin>){
                    if constexpr (is_based_on<self_management, origin>){
                        if constexpr (tin<guide, origin>){
                            can_arrive_root = xroot.template routing<guide>();
                        }
                    }
                    else{
                        using member_tuple = tuple<origin, typename origin::member_list>;
                        can_arrive_root = cast<member_tuple>(xroot).template routing<guide>();
                    }
                }

                xdebug(im_gc_tuple_routing, xtypeid(origin).name, can_arrive_root);
                can_arrive_root |= next.template routing<guide>();
                return can_arrive_root;
            }
        };

        template<class root_t>
        union tuple<root_t, vlist<>>{
            template<class guide> bool routing(){
                return false;
            }
        };
    }
    #undef  xroot
    #pragma pop_macro("xuser")
#endif

namespace xuser::inc{
    using namespace ::mixc::gc_tuple::origin;
}
