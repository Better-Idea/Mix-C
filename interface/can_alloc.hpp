#ifndef xpack_interface_can_alloc
#define xpack_interface_can_alloc
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::interface_can_alloc
        #include"define/base_type.hpp"
        #include"interface/can_callback.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::interface_can_alloc{
        template<class item>
        using can_alloc = inc::can_callback<item *(uxx length)>;
    }

#endif

namespace xuser::inc{
    using ::mixc::interface_can_alloc::can_alloc;
}
