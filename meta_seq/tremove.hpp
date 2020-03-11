#pragma once
#include"meta/is_same.hpp"
#include"meta_seq/tlist.hpp"

namespace mixc {
    template<class list, class item>
    struct tremove{
    private:
        template<class first, class ... args, class ... result_args>
        static auto invoke(tlist<first, args...>, tlist<result_args...>){
            if constexpr(is_same<first, item>){
                return invoke(tlist<args...>(), tlist<result_args...>());
            }
            else{
                return invoke(tlist<args...>(), tlist<result_args..., first>());
            }
        }

        template<class result>
        static auto invoke(tlist<>, result r){
            return r;
        }
    public:
        using new_list = decltype(invoke(list(), tlist<>()));
    };
}
