#pragma once
#include"utility/type/implicit.hpp"
#include"lang/cxx.index_of_last_miss.hpp"
#include"lang/cxx.clone.hpp"
#include"lang/private/cxx.trim.hpp"
#include"memory/alloc_callback.hpp"
#include"meta/is_same.hpp"
// #include"cxx.hpp"

namespace mixc{
    template<class item> struct cxx;
    template<class item, class ... args>
    inline cxx<item> trim_end(cxx<item> self, item value, args const & ... list) {
        item group[sizeof...(args) + 1]; // 包含'\0'
        auto may_alloc = inner_trim::layout_args(group, value, list...);
        constexpr auto need_alloc = is_same<decltype(may_alloc), alloc_callback<item>>;
        auto token = cxx<item>{ group, sizeof...(args) + 1 - need_alloc };
        self.length = mixc::index_of_last_miss<item>(
            self, 
            token,
            token.length
        );

        if constexpr (need_alloc){
            if (self.length == not_exist){
                return cxx<item>();
            }
            else{
                return mixc::clone<item>(
                    self,
                    may_alloc
                );
            }
        }
        else {
            return self;
        }
    }
}
