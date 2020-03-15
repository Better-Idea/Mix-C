#pragma once
#include"define/base_type.hpp"
#include"self_management.hpp"
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
        uxx routing(guide gui, voidp root){
            uxx irc = 0;
            if constexpr (is_class<first>){
                if constexpr (is_based_on<self_management, first>){
                    if constexpr (tin<guide, first>){
                        irc = item.routing(gui, root);
                    }
                }
                else{
                    using tuplep = tuple<typename first::gc_candidate_list> *;
                    irc = tuplep(addressof(item))->routing(gui, root);
                }
            }
            return irc + next.routing(gui,root);
        }

        template<class guide>
        void clear_footmark(guide gui){
            if constexpr (is_class<first>){
                if constexpr (is_based_on<self_management, first>){
                    if constexpr (tin<guide, first>){
                        item.clear_footmark(gui);
                    }
                }
                else{
                    using tuplep = tuple<typename first::gc_candidate_list> *;
                    tuplep(addressof(item))->clear_footmark(gui);
                }
            }
            next.clear_footmark(gui);
        }
    };

    template<>
    struct tuple<tlist<>>{
        template<class guide>
        constexpr uxx routing(guide, void *){
            return 0;
        }

        template<class guide>
        constexpr void clear_footmark(guide){}
    };
}
