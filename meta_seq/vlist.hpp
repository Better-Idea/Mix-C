#ifndef xpack_meta_seq_vlist
#define xpack_meta_seq_vlist
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::meta_seq_vlist
        #include"define/base_type.hpp"
        #include"meta_seq/tlist.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::meta_seq_vlist{
        template<auto ... values>
        struct vlist{
            using type_list = inc::tlist<decltype(values)...>;
            static constexpr uxx length = sizeof...(values);
        };
    }

#endif

namespace xuser::inc{
    using ::mixc::meta_seq_vlist::vlist;
}
