#pragma once
#include"meta_seq/tlist.hpp"
#include"meta_seq/tin.hpp"

namespace mixc {
    template<class list_a, class list_b>
    struct tdistinct_marge{
    private:
        template<class first, class ... args, class ... result_args>
        static auto invoke(tlist<first, args...>, tlist<result_args...>){
            if constexpr(tin<tlist<result_args...>, first>){
                return invoke(
                    tlist<args...>(), 
                    tlist<result_args...>()
                );
            }
            else{
                return invoke(
                    tlist<args...>(), 
                    tlist<result_args..., first>()
                );
            }
        }

        template<class result_list>
        static auto invoke(tlist<>, result_list r){
            return r;
        }
    public:
        using new_list = decltype(
            invoke(list_b(), list_a())
        );
    };
}
