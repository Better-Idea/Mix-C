#ifndef xpack_meta_item_of
#define xpack_meta_item_of
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_item_of
#include"define/base_type.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_item_of{
    template<class seq_t>
    using item_of = decltype(((seq_t *)nullptr)[0][0]);
}

#endif

namespace xuser::inc{
    using ::mixc::meta_item_of::item_of;
}
