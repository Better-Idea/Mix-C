#ifndef xpack_interface_can_random_access
#define xpack_interface_can_random_access
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::interface_can_random_access
        #include"define/base_type.hpp"
        #include"macro/xitf.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::interface_can_random_access{
        template<class item>
        xitf(can_random_access)
            xitf_item(operator [], item &,
                xnt(index, uxx)
            );
            xitf_item(length, uxx);
        xitf_end();
    }

#endif

namespace xuser::inc{
    using ::mixc::interface_can_random_access::can_random_access;
    using namespace ::mixc::macro_private_callable;
}
