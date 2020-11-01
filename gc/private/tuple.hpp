#ifndef xpack_gc_private_tuple
#define xpack_gc_private_tuple
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::gc_private_tuple::inc
#include"gc/self_management.hpp"
#include"macro/xdebug.hpp"
#include"meta/is_based_on.hpp"
#include"meta/remove_const.hpp"
#include"meta/remove_ref.hpp"
#include"meta/remove_membership.hpp"
#include"meta_seq/tlist.hpp"
#include"meta_seq/vlist.hpp"
#include"meta_seq/tin.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::gc_private_tuple{
    using namespace inc;

    template<class root_t>
    struct tuple{
        using base_list             = typename root_t::base_list;
        using member_list_partial   = typename root_t::member_list_partial;
        
        template<class guide>
        bool routing(){
            bool can_arrive_root = false;
            can_arrive_root |= routing_base<guide>(base_list());
            can_arrive_root |= routing_self<guide>(member_list_partial());
            return can_arrive_root;
        }

    private:
        template<class guide, auto a0, auto ... args>
        bool routing_self(vlist<a0, args...>){
            using origin = remove_membership<decltype(a0)>;
            bool can_arrive_root = false;

            if constexpr (tin<guide, origin>){
                auto field = xref ((root_t *)this)->*a0;

                if constexpr (is_based_on<self_management, origin>){
                    can_arrive_root |= field->template routing<guide>();
                }
                else{
                    auto entry = (tuple<origin> *)field;
                    can_arrive_root |= entry->template routing<guide>();
                }
            }
            return can_arrive_root | routing_self<guide>(vlist<args...>());
        }

        template<class guide>
        bool routing_self(vlist<>){
            return false;
        }

        template<class guide, class base0, class ... basex>
        bool routing_base(tlist<base0, basex...>){
            auto child           = (root_t *)this;
            auto base            = (base0 *)child; // 李氏转换
            auto new_tuple       = (tuple<base0> *)base;
            auto can_arrive_root = 
                new_tuple->template routing<guide>() | 
                routing_base<guide>(tlist<basex...>());
            return can_arrive_root;
        }

        template<class guide>
        bool routing_base(tlist<>){
            return false;
        }
    };

    template<>
    struct tuple<void>{
        template<class guide, class array>
        static bool routing(array & list, uxx length){
            using item_t = remove_const<
                remove_ref<decltype(list[0])>
            >;
            using tuplep = tuple<item_t> *;
            bool can_arrive_root = false;

            if constexpr (is_based_on<self_management, item_t>){
                for(uxx i = 0; i < list.length(); i++){
                    can_arrive_root |= list[i].template routing<guide>();
                }
            }
            else{
                for(uxx i = 0; i < list.length(); i++){
                    can_arrive_root |= tuplep(xref list[i])->template routing<guide>();
                }
            }
            return can_arrive_root;
        }
    };
}

#endif

xexport(mixc::gc_private_tuple::tuple)