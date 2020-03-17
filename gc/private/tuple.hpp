#pragma once
#include"define/base_type.hpp"
#include"gc/private/self_management.hpp"
#include"gc/private/routing_result.hpp"
#include"memop/addressof.hpp"
#include"meta/is_class.hpp"
#include"meta/is_based_on.hpp"
#include"meta_seq/tlist.hpp"
#include"meta_seq/tin.hpp"

namespace mixc::inner_gc{
    template<class list> struct tuple;
    template<class first, class ... args>
    struct tuple<tlist<first, args...>>{
    private:
        first item;
        tuple<tlist<args...>> next;
    public:
        template<class ... args_dummy>
        tuple(args_dummy const & ... list) : 
            item(list...), next(list...){
        }

        template<class guide>
        routing_result routing(guide gui){
            routing_result r = { 0, 0 };
            if constexpr (is_class<first>){
                if constexpr (is_based_on<self_management, first>){
                    if constexpr (tin<guide, first>){
                        // printf("%s | routing\n", typeid(first).name());
                        if (r = item.routing(gui); r.can_arrive_root) {
                            r.degree_dvalue -= 1;
                            // printf("%s | routing io:%lld\n", typeid(first).name(), r.degree_dvalue);
                        }
                    }
                }
                else{
                    using tuplep = tuple<typename first::gc_candidate_list> *;
                    r = tuplep(addressof(item))->routing(gui);
                }
            }
            
            routing_result n = next.routing(gui);
            r.can_arrive_root |= n.can_arrive_root;
            r.degree_dvalue += n.degree_dvalue;
            return r;
        }

        template<class guide>
        void clear_footmark(guide gui, voidp root){
            if constexpr (is_class<first>){
                if constexpr (is_based_on<self_management, first>){
                    if constexpr (tin<guide, first>){
                        // printf("%s | clear\n", typeid(first).name());
                        item.clear_footmark(gui, root);
                    }
                }
                else{
                    using tuplep = tuple<typename first::gc_candidate_list> *;
                    tuplep(addressof(item))->clear_footmark(gui, root);
                }
            }
            next.clear_footmark(gui, root);
        }
    };

    template<>
    struct tuple<tlist<>>{
        template<class guide>
        constexpr routing_result routing(guide){
            return { 0, 0 };
        }

        template<class guide>
        constexpr void clear_footmark(guide, voidp){}
    };
}
