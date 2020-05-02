#ifndef xpack_meta_seq_tpop_front
#define xpack_meta_seq_tpop_front
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::meta_seq_tpop_front
        #include"define/base_type.hpp"
        #include"meta_seq/tlist.hpp"
        #include"meta_seq/tnull.hpp"
    #pragma pop_front_macro("xuser")

    namespace mixc::meta_seq_tpop_front{
        template<class list> struct tpop_front;
        template<class first, class ... args>
        struct tpop_front<inc::tlist<first, args...>>{
            using item = first;
            using new_list = inc::tlist<args...>;
        };

        template<>
        struct tpop_front<inc::tlist<>>{
            using item = inc::tnull;
            using new_list = inc::tlist<>;
        };
    }

#endif

namespace xuser::inc{
    using ::mixc::meta_seq_tpop_front::tpop_front;
}
