#pragma once
#include"macro/private/xlist.hpp"
#include"macro/private/xprefix.hpp"
#include"meta_seq/tlist.hpp"
#include"meta_seq/tmarge.hpp"
#include"utility/type/dummy_t.hpp"

namespace mixc{
    template<class a0 = dummy_t, class ... args>
    inline auto expand_member_list(){
        using current = typename a0::member_list;
        if constexpr (sizeof...(args) == 0){
            return current();
        }
        else{
            using next = decltype(expand_member_list<args...>());
            using set = typename tmarge<current, next>::new_list;
            return set();
        }
    }
}

#define xgc(name,...)                                                               \
struct name :                                                                       \
    private mixc::dummy_t                                                           \
    __xlist__(base_,base_,__VA_ARGS__) {                                            \
private:                                                                            \
    using __self__ =  name __xlist__(none_, none_, __VA_ARGS__);                    \
    using __expand_member_list__ =                                                  \
        decltype(                                                                   \
            mixc::expand_member_list<mixc::dummy_t                                  \
                __xlist__(member_header_,member_,__VA_ARGS__)                       \
            >()                                                                     \
        );                                                                          \
public:                                                                             \
    template<class expand>                                                          \
    operator expand &() const {                                                     \
        static_assert(__is_base_of(__self__, expand));                              \
        return *(expand *)this;                                                     \
    }

#define xgc_fields(...)                                                             \
    __xlist__(field_,field_,__VA_ARGS__);                                           \
    using member_list = typename mixc::tmarge<                                      \
        __expand_member_list__,                                                     \
        mixc::tlist<                                                                \
            __xlist__(member_list_header_,member_list_,__VA_ARGS__)                 \
        >                                                                           \
    >::new_list

#define xgc_end()       }
