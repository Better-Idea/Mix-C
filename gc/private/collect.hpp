/*
在 GC 组件中，collect 模板类将 routing 的结果作为输入，输出构成环形引用的类型集合
我们定义 collect 的模板参数 kvlist 作为连接关系集合， result_list 作为结果集合，
search_list 期初是一个只有根类型 root 的集合。
1.  从 search_list 中取出一个元素 first，若无可取的元素，则遍历结束
2.  若 first 不在 result_list，就将 first 添加到 result_list 中，否则跳到操作 1
3.  将 kvlist 中与 first 相连接的类型取出并放入搜索集合 search_list
4.  跳到操作 1
*/

#ifndef xpack_gc_collect
#define xpack_gc_collect
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::gc_collect
        #include"define/base_type.hpp"
        #include"meta/remove_membership.hpp"
        #include"meta_seq/tdistinct_append.hpp"
        #include"meta_seq/tfilter.hpp"
        #include"meta_seq/tin.hpp"
        #include"meta_seq/tkv.hpp"
        #include"meta_seq/tlist.hpp"
        #include"meta_seq/tmarge.hpp"
        #include"meta_seq/tpop_by.hpp"
        #include"meta_seq/tselector_key.hpp"
        #include"meta_seq/tselector_val.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::gc_collect{
        using namespace inc;

        template<class root, class kvlist>
        struct collect{
        private:
            template<class current_kvlist, class result_list, class first, class ... args>
            static auto invoke(current_kvlist kv, tlist<first, args...>, result_list result){
                using pair              = tpop_by<current_kvlist, first, tselector_key>;
                using item_kvlist       = typename pair::item_list;

                if constexpr(tin<result_list, first> or item_kvlist::length == 0){
                    return invoke(kv, tlist<args...>(), result);
                }
                else{
                    using rest_kvlist       = typename pair::new_list;
                    using parents_list      = typename tfilter<item_kvlist, tselector_val>::new_list;
                    using new_children_list = typename tmarge<tlist<args...>, parents_list>::new_list;
                    using new_result        = typename tdistinct_append<result_list, first>::new_list;
                    return invoke(
                        rest_kvlist(),
                        new_children_list(),
                        new_result()
                    );
                }
            }

            template<class current_kvlist, class result_list>
            static auto invoke(current_kvlist, tlist<>, result_list r){
                return r;
            }
        public:
            using result_list = decltype(
                invoke(kvlist(), tlist<root>(), tlist<>())
            );
        };
    }

#endif

namespace xuser::inc{
    using ::mixc::gc_collect::collect;
}
