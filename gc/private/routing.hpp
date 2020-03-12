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
        template<class current, class children_list>
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
                invoke(children_list(), tlist<>())
            );
        };

        template<class active_list, class result_kvlist, class visited_list>
        static auto invoke(active_list, result_kvlist, visited_list){
            using pair = tdeque<active_list>;
            using new_root = typename pair::item;
            using current_active_list = typename pair::new_list;
            
            if constexpr(not is_class<new_root> or tin<visited_list, new_root>){
                return invoke(current_active_list(), result_kvlist(), visited_list());
            }
            else{
                using children_list = typename new_root::gc_candidate_list;
                using current_kvlst = typename attach<new_root, children_list>::new_list;
                using new_active_list = typename tmarge<current_active_list, children_list>::new_list;
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
