
#ifndef xpack_meta_seq_tdeque
#define xpack_meta_seq_tdeque
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::meta_seq_tdeque
        #include"define/base_type.hpp"
        #include"meta_seq/tnull.hpp"
        #include"meta_seq/tlist.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::meta_seq_tdeque{
        using namespace inc;

        template<class tlist> struct tdeque;
        template<class first, class ... args>
        struct tdeque<tlist<first, args...>>{
            using item = first;
            using new_list = tlist<args...>;
        };

        template<>
        struct tdeque<tlist<>>{
            using item = tnull;
            using new_list = tlist<>;
        };
    }

#endif

namespace xuser::inc{
    using ::mixc::meta_seq_tdeque::tdeque;
}
