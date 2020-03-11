#pragma once
#include"meta_seq/tlist.hpp"

namespace mixc{
    template<class list, template <class> class selector>
    struct tfilter{
    private:
        template<class ... result_args>
        static auto invoke(tlist<>, tlist<result_args...> r){
            return r;
        }

        template<class first, class ... args, class ... result_args>
        static auto invoke(tlist<first, args...>, tlist<result_args...>){
            using item = typename selector<first>::item;
            return invoke(
                tlist<args...>(), 
                tlist<result_args..., item>()
            );
        }
    public:
        using new_list = decltype(
            invoke(list(), tlist<>())
        );
    };
}
