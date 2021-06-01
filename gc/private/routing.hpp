/*
在 GC 组件中，routing 模板类从根类型开始路由，
输出类型和类型之间的连接关系，我们通过 tkv<> 类型键值对存放这种连接关系，
通过 tlist<> 类型列表存放连接关系的集合。
比如：类型 A 含有成员类型 B，则将这种关系以 tkv<B, A> 的形式保存
说明：采用这种方式保存方便通过 bfs（宽度优先搜索算法）筛选出所有构成环形引用的类型组
*/

#ifndef xpack_gc_private_routing
#define xpack_gc_private_routing
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::gc_private_routing::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"meta/is_class.hpp"
#include"meta_seq/tappend.hpp"
#include"meta_seq/tdeque.hpp"
#include"meta_seq/tin.hpp"
#include"meta_seq/tkv.hpp"
#include"meta_seq/tlist.hpp"
#include"meta_seq/tmerge.hpp"
#pragma pop_macro("xuser")

namespace mixc::gc_private_routing{
    using namespace inc;

    template<class root_t>
    struct routing{
    private:
        template<class meta_t> struct origin {
            using type = meta_t;
        };

        template<class object_t, class meta_t> struct origin<meta_t object_t::*> {
            using type = meta_t;
        };

        template<class object_t, class meta_t> struct origin<meta_t * object_t::*> {
            using type = meta_t;
        };

        // attach 用于将 current_t 与它的成员类型列表 member_list_t 中的每一个成员类型添加连接关系
        // member_list_t 的所有成员都被 current_t 包含
        // 所以对于任意成员类型 first，都有 tkv<first, current_t> 连接关系
        // 此外我们排除不是类类型 first 构成的 tkv<>，因为非类类型的成员类型是无法构成环形引用
        template<class current_t, class member_list_t>
        struct attach{
        private:
            template<class first, class ... args_t, class ... result_args>
            static auto invoke(tlist<first, args_t...>, tlist<result_args...>){
                using without_membership = typename origin<first>::type;

                if constexpr (is_class<without_membership>){
                    return invoke(
                        tlist<args_t...>(),
                        tlist<result_args..., tkv<without_membership, current_t>>()
                    );
                }
                else{
                    return invoke(
                        tlist<args_t...>(),
                        tlist<result_args...>()
                    );
                }
            }

            template<class result>
            static auto invoke(tlist<>, result r){
                return r;
            }
        public:
            using new_list = decltype(
                invoke(member_list_t(), tlist<>())
            );
        };

        // 通过 bfs 算法得到连接关系的集合 tlist<tkv<>>
        template<class active_list_t, class result_kvlist_t, class visited_list_t>
        static auto invoke(active_list_t, result_kvlist_t, visited_list_t){
            using pair                          = tdeque<active_list_t>;
            using new_root                      = typename pair::item_t;
            using new_root_without_membership   = typename origin<new_root>::type;
            using current_active_list           = typename pair::new_list;
            
            if constexpr (
                is_class<new_root_without_membership> == false or 
                tin<visited_list_t, new_root_without_membership>){
                return invoke(current_active_list(), result_kvlist_t(), visited_list_t());
            }
            else{
                using member_list               = typename new_root_without_membership::member_list::type_list;
                using current_kvlst             = typename attach<new_root_without_membership, member_list>::new_list;
                using new_active_list           = typename tmerge<current_active_list, member_list>::new_list;
                using new_visited_list          = typename tappend<visited_list_t, new_root_without_membership>::new_list;
                using new_result_kvlist         = typename tmerge<result_kvlist_t, current_kvlst>::new_list;
                return invoke(new_active_list(), new_result_kvlist(), new_visited_list());
            }
        }

        template<class result_kvlist_t, class visited_list_t>
        static auto invoke(tlist<>, result_kvlist_t, visited_list_t){
            return result_kvlist_t();
        }
    public:
        using result_kvlist = decltype(
            invoke(tlist<root_t>(), tlist<>(), tlist<>())
        );
    };
}

#endif

xexport(mixc::gc_private_routing::routing)