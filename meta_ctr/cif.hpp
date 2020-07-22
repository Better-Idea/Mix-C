#ifndef xpack_meta_ctr_cif
#define xpack_meta_ctr_cif
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_ctr_cif
#include"define/base_type.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_ctr_cif{
    template<bool condition, class if_true, class if_false>
    struct meta{
        using result = if_true;
    };

    template<class if_true, class if_false>
    struct meta<false, if_true, if_false>{
        using result = if_false;
    };

    template<bool condition, class if_true, class if_false>
    using cif = typename meta<condition, if_true, if_false>::result;
}

#endif

namespace xuser::inc{
    using ::mixc::meta_ctr_cif::cif;
}
