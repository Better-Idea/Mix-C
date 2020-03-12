#pragma once
#include"meta/is_class.hpp"
#include"meta_seq/tdistinct_marge.hpp"
#include"meta_seq/tfilter.hpp"
#include"meta_seq/tin.hpp"
#include"meta_seq/tkv.hpp"
#include"meta_seq/tlist.hpp"
#include"meta_seq/tmarge.hpp"
#include"meta_seq/tpop_by.hpp"
#include"meta_seq/tselector_key.hpp"
#include"meta_seq/tselector_val.hpp"

namespace mixc::inner_gc{
    template<class root, class kvlist>
    struct collect{
    private:
        template<class current_kvlist, class result_list, class first, class ... args>
        static auto invoke(current_kvlist kv, tlist<first, args...> childrens, result_list result){
            if constexpr(tin<result_list, typename first::val){
                return invoke(kv, tlist<args...>(), result);
            }
            else{
                using pair              = tpop_by<current_kvlist, first, tselector_key>;
                using rest_kvlist       = typename pair::new_list;
                using item_kvlist       = typename pair::item_list;
                using parents_list      = typename tfilter<item_kvlist, tselector_val>::new_list;
                using new_children_list = typename tmarge<tlist<args...>, parents_list>::new_list;
                using new_result        = typename tdistinct_marge<result_list, parents_list>::new_list;
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
