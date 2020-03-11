#pragma once
#include"meta/is_same.hpp"
#include"meta_seq/tlist.hpp"

namespace mixc{
    namespace inner_tin{
        template<class item>
        inline constexpr bool meta(tlist<>){
            return false;
        }

        template<class item, class first, class ... args>
        inline constexpr bool meta(tlist<first, args...>){
            if constexpr (mixc::is_same<first, item>){
                return true;
            }
            else{
                return meta<item>(tlist<args...>());
            }
        }
    }
    template<class tlist, class item>
    constexpr bool tin = mixc::inner_tin::meta<item>(tlist());
}
