#ifndef xpack_meta_item_remove_ref_of
#define xpack_meta_item_remove_ref_of
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_item_remove_ref_of
#include"define/base_type.hpp"
#include"meta/remove_ref.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_item_remove_ref_of{
    template<class seq_t>
    using item_remove_ref_of = 
        inc::remove_ref<
            decltype(((seq_t *)nullptr)[0][0])
        >;
}

#endif

namespace xuser::inc{
    using ::mixc::meta_item_remove_ref_of::item_remove_ref_of;
}
