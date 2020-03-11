#pragma once
#include"meta_seq/tlist.hpp"
#include"meta_seq/tin.hpp"

namespace mixc {
    template<class list>
    struct tdistinct{
    private:
        template<class first, class ... args, class ... result_args>
        static auto invoke(tlist<first, args...>, tlist<result_args...>){
            using old_list = tlist<result_args...>;
            using new_list = tlist<result_args..., first>;
            if constexpr(tin<tlist<result_args...>, first>){
                return invoke(
                    tlist<args...>(),
                    old_list()
                );
            }
            else{
                return invoke(
                    tlist<args...>(),
                    new_list()
                );
            }
        }

        template<class result>
        static auto invoke(tlist<>, result r){
            return r;
        }
    public:
        using new_list = decltype(
            invoke(list(), tlist<>())
        );
    };
}
