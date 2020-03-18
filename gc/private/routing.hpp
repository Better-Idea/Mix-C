/*
在 GC 组件中，routing 模板类从根类型开始路由，
输出类型和类型之间的连接关系，我们通过 tkv<> 类型键值对存放这种连接关系，
通过 tlist<> 类型列表存放连接关系的集合。
比如：类型 A 含有成员类型 B，则将这种关系以 tkv<B, A> 的形式保存
说明：采用这种方式保存方便通过 bfs（宽度优先搜索算法）筛选出所有构成环形引用的类型组
*/

#pragma once
#include"meta/is_class.hpp"
#include"meta_seq/tappend.hpp"
#include"meta_seq/tdeque.hpp"
#include"meta_seq/tlist.hpp"
#include"meta_seq/tkv.hpp"
#include"meta_seq/tin.hpp"
#include"meta_seq/tmarge.hpp"

namespace mixc::inner_gc{
    template<class root>
    struct routing{
    private:
        // attach 用于将 current 与它的成员类型列表 member_list 中的每一个成员类型添加连接关系
        // member_list 的所有成员都被 current 包含
        // 所以对于任意成员类型 first，都有 tkv<first, current> 连接关系
        // 此外我们排除不是类类型 first 构成的 tkv<>，因为非类类型的成员类型是无法构成环形引用
        template<class current, class member_list>
        struct attach{
        private:
            template<class first, class ... args, class ... result_args>
            static auto invoke(tlist<first, args...>, tlist<result_args...>){
                if constexpr(is_class<first>){
                    return invoke(
                        tlist<args...>(),
                        tlist<result_args..., tkv<first, current>>()
                    );
                }
                else{
                    return invoke(
                        tlist<args...>(),
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
                invoke(member_list(), tlist<>())
            );
        };

        // 通过 bfs 算法得到连接关系的集合 tlist<tkv<>>
        template<class active_list, class result_kvlist, class visited_list>
        static auto invoke(active_list, result_kvlist, visited_list){
            using pair = tdeque<active_list>;
            using new_root = typename pair::item;
            using current_active_list = typename pair::new_list;
            
            if constexpr(not is_class<new_root> or tin<visited_list, new_root>){
                return invoke(current_active_list(), result_kvlist(), visited_list());
            }
            else{
                using member_list = typename new_root::member_list;
                using current_kvlst = typename attach<new_root, member_list>::new_list;
                using new_active_list = typename tmarge<current_active_list, member_list>::new_list;
                using new_visited_list = typename tappend<visited_list, new_root>::new_list;
                using new_result_kvlist = typename tmarge<result_kvlist, current_kvlst>::new_list;
                return invoke(new_active_list(), new_result_kvlist(), new_visited_list());
            }
        }

        template<class result_kvlist, class visited_list>
        static auto invoke(tlist<>, result_kvlist, visited_list){
            return result_kvlist();
        }
    public:
        using result_kvlist = decltype(
            invoke(tlist<root>(), tlist<>(), tlist<>())
        );
    };
}
