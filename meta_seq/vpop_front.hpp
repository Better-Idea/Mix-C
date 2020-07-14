#ifndef xpack_meta_seq_vpop_front
#define xpack_meta_seq_vpop_front
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_seq_vpop_front
#include"define/base_type.hpp"
#include"meta_seq/vlist.hpp"
#include"meta_seq/vnull.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_seq_vpop_front{
    template<class list> struct vpop_front;
    template<auto first, auto ... args>
    struct vpop_front<inc::vlist<first, args...>>{
        static constexpr auto item  = first;
        using new_list              = inc::vlist<args...>;
    };

    template<>
    struct vpop_front<inc::vlist<>>{
        static constexpr auto item  = inc::vnull;
        using new_list              = inc::vlist<>;
    };
}

#endif

namespace xuser::inc{
    using ::mixc::meta_seq_vpop_front::vpop_front;
}
